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
#include <flatbuf/main_generated.h>
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
using Sumeragi = ::iroha::Sumeragi;
using ConsensusEvent = ::iroha::ConsensusEvent;
using Response = ::iroha::Response;
using Transaction = ::iroha::Transaction;
using Signature = ::iroha::Signature;

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
  RESPONSE_INVALID_SIG,  // wrong signature
  RESPONSE_ERRCONN,      // connection error
};

/************************************************************************************
 * Verify
 ************************************************************************************/
namespace iroha {
namespace SumeragiImpl {
namespace Verify {

ScheduledTaskToConsensus<Verify::CallBackFunc> scheduledTask;

/**
 * Receive callback
 */
void receive(Verify::CallBackFunc&& callback) {
  scheduledTask.set(std::move(callback));
}

bool send(const std::string& ip, const std::unique_ptr<ConsensusEvent>& event) {
  // ToDo
  if (config::PeerServiceConfig::getInstance().isExistIP(ip)) {
    return true;
  }
  return false;
}

bool sendAll(const std::unique_ptr<ConsensusEvent>& event) {
  auto receiver_ips = config::PeerServiceConfig::getInstance().getGroup();
  for (const auto& p : receiver_ips) {
    if (p["ip"].get<std::string>() !=
        config::PeerServiceConfig::getInstance().getMyIpWithDefault("AA")) {
      send(p["ip"].get<std::string>(), event);
    }
  }
  return true;
}
}
}
}  // namespace iroha::SumeragiImpl::Verify

/************************************************************************************
 * Torii
 ************************************************************************************/
namespace iroha {
namespace SumeragiImpl {
namespace Torii {

ScheduledTaskToConsensus<Torii::CallBackFunc> scheduledTask;

void receive(Torii::CallBackFunc&& callback) {
  scheduledTask.set(std::move(callback));
}
}
}
}  // namespace iroha::SumeragiImpl::Torii

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


}  // namespace connection
