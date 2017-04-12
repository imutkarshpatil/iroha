/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <flatbuffers/flatbuffers.h>
#include <grpc++/grpc++.h>
#include <consensus/connection/connection.hpp>
#include <crypto/signature.hpp>
#include <infra/config/iroha_config_with_json.hpp>
#include <infra/config/peer_service_with_json.hpp>
#include <service/peer_service.hpp>
#include <util/exception.hpp>
#include <util/logger.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace connection {
/**
 * Using
 */
using Sumeragi        = ::iroha::Sumeragi;
using ConsensusEvent  = ::iroha::ConsensusEvent;
using Response        = ::iroha::Response;
using Transaction     = ::iroha::Transaction;
using Signature       = ::iroha::Signature;

using grpc::Channel;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ClientContext;
using grpc::Status;

/**
 * Enum
 */
enum ResponseType {
  RESPONSE_OK,
  RESPONSE_INVALID_SIG, // wrong signature
  RESPONSE_ERRCONN,     // connection error
};

/**
 * Store callback function
 */
template<class CallBackFunc>
class Receiver {
public:
  void set(CallBackFunc&& rhs) {
    if (receiver_) {
      throw exception::DuplicateSetException(
          "Receiver<" + std::string(typeid(CallBackFunc).name()) + ">", __FILE__);
    }
    receiver_ = std::make_shared<CallBackFunc>(rhs);
  }

  template<typename... Args>
  void call(Args&&... args) {
    if (!receiver_) {
      std::cout << "owariowariowari\n";
      exit(1);
    }
    (*receiver_)(std::forward<Args>(args)...);
    receiver_.reset();
  }

private:
  std::shared_ptr<CallBackFunc> receiver_;
};

/************************************************************************************
 * Verify
 ************************************************************************************/
namespace iroha { namespace SumeragiImpl { namespace Verify {

Receiver<Verify::CallBackFunc> receiver;

/**
 * Receive callback
 */
void receive(Verify::CallBackFunc&& callback) {
  receiver.set(std::move(callback));
}

bool send(const std::string& ip, const ConsensusEvent& event) {
  // ToDo
  /*
  auto receiver_ips = config::PeerServiceConfig::getInstance().getIpList();
  if (find(receiver_ips.begin(), receiver_ips.end(), ip) != receiver_ips.end())
  { return true; } else { return false; }
  */
  return true;
}

bool sendAll(const ConsensusEvent& event) {
  // ToDo
  /*
  auto receiver_ips = config::PeerServiceConfig::getInstance().getIpList();
  for (auto &ip : receiver_ips) {
      // ToDo
      if (ip != config::PeerServiceConfig::getInstance().getMyIp()) {
          send(ip, std::move(event));
      }
  }
  */
  return true;
}

}}} // namespace iroha::SumeragiImpl::Verify


/************************************************************************************
 * Torii
 ************************************************************************************/
namespace iroha { namespace SumeragiImpl { namespace Torii {

Receiver<Torii::CallBackFunc> receiver;

void receive(Torii::CallBackFunc&& callback) {
  receiver.set(std::move(callback));
}

}}} // namespace iroha::SumeragiImpl::Torii

/************************************************************************************
 * Connection Client
 ************************************************************************************/
class SumeragiConnectionClient {
public:
  explicit SumeragiConnectionClient(std::shared_ptr<Channel> channel)
      : stub_(Sumeragi::NewStub(channel)) {}

  Response Verify(const ConsensusEvent& consensusEvent) {
    Response response;
    logger::info("connection") << "Operation";
    logger::info("connection")
        << "size: " << consensusEvent.eventsignatures_size();
    logger::info("connection")
        << "name: " << consensusEvent.transaction().asset().name();
    ClientContext context;

    Status status = stub_->Verify(&context, consensusEvent, &response);

    if (status.ok()) {
        logger::info("connection")  << "response: " << response.value();
        //return {response.value(), valid(response.confirm()) ? RESPONSE_OK : RESPONSE_INVALID_SIG}; } else { logger::error("connection") << status.error_code() << ": " << status.error_message();
        //std::cout << status.error_code() << ": " << status.error_message();
        //return {"RPC failed", RESPONSE_ERRCONN};
    }
  }

  Response Torii(const Transaction& transaction) {
    Response response;
    ClientContext context;
    Status status = stub_->Torii(&context, transaction, &response);

    if (status.ok()) {
        logger::info("connection")  << "response: " << response.value();
        return {response.value(), RESPONSE_OK};
    } else {
        logger::error("connection") << status.error_code() << ": " <<
    status.error_message();
        //std::cout << status.error_code() << ": " << status.error_message();
        return {"RPC failed", RESPONSE_ERRCONN};
    }
  }

 private:
  std::unique_ptr<Sumeragi::Stub> stub_;
};

/************************************************************************************
 * Connection Service
 ************************************************************************************/
class SumeragiConnectionServiceImpl final : public ::iroha::Sumeragi::Service {
public:
  Status Verify(ServerContext* context,
                const flatbuffers::BufferRef<ConsensusEvent>* request,
                flatbuffers::BufferRef<Response>* response) override {


    return grpc::Status::OK;
  }

  Status Torii(ServerContext* context,
               const flatbuffers::BufferRef<Transaction>* transaction,
               flatbuffers::BufferRef<Response>* response) override {
    // Extract Transaction from BufferRef<Transaction>
    auto txRawPtr = transaction->GetRoot();
    auto txPtr = std::make_unique<Transaction>(std::move(*txPtr));
    txRawPtr = nullptr;

    // Push new processTransaction() task to pool
    receiver.call("", std::move(txPtr));  // currently, from = "" (dummy)

    // Create signature
    auto signatures = transaction->signatures(); // TODO: What should be?

    // Create response
    fbb.Clear();

    auto resOffset = ::iroha::CreateResponseDirect(
        fbb,
        /*message*/     "OK",
        /*iroha::Code*/ iroha::Code_COMMIT,
        /*signature*/   signature->Get(0));

    fbb.Finish(resOffset);

    *response = flatbuffers::BufferRef<::iroha::Response>(
        fbb.GetBufferPointer(), fbb.GetSize());

    return grpc::Status::OK;
  }

private:
  flatbuffers::FlatbufferBuilder fbb;
};

/************************************************************************************
 * Main connection
 ************************************************************************************/
std::mutex wait_for_server;
ServerBuilder builder;
grpc::Server* server = nullptr;
std::condition_variable server_cv;

void initialize_peer() {

  // ToDo catch exception of to_string


  logger::info("Connection GRPC")  <<" initialize_peer ";
}

int run() {
  logger::info("Connection GRPC")  <<" RUN ";
    auto address =
        "0.0.0.0:" +
        std::to_string(
                config::IrohaConfigManager::getInstance().getGrpcPortNumber(50051)
        );
    SumeragiConnectionServiceImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

  wait_for_server.lock();
  server = builder.BuildAndStart().release();
  wait_for_server.unlock();
  server_cv.notify_one();

  server->Wait();
  return 0;
}

void finish() {
  server->Shutdown();
  delete server;
}


// using Response = std::pair<std::string, ResponseType>;
/*
// TODO: very dirty solution, need to be out of here
std::function<RecieverConfirmation(const std::string&)> sign = [](const
std::string &hash) { RecieverConfirmation confirm; Signature signature;
    signature.set_publickey(config::PeerServiceConfig::getInstance().getMyPublicKey());
    signature.set_signature(signature::sign(
            config::PeerServiceConfig::getInstance().getMyPublicKey(),
            hash,
            config::PeerServiceConfig::getInstance().getMyPrivateKey())
    );
    confirm.set_hash(hash);
    confirm.mutable_signature()->Swap(&signature);
    return confirm;
};

std::function<bool(const RecieverConfirmation&)> valid = [](const
RecieverConfirmation &c) { return signature::verify(c.signature().signature(),
c.hash(), c.signature().publickey());
};
*/

} // namespace connection
