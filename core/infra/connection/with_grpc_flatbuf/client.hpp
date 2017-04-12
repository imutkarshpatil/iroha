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

#ifndef CORE_CONNECTION_WITH_GRPC_FLATBUF_CLIENT_HPP
#define CORE_CONNECTION_WITH_GRPC_FLATBUF_CLIENT_HPP

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

class SumeragiConnectionClient {
 public:
  explicit SumeragiConnectionClient(std::shared_ptr<Channel> channel)
      : stub_(Sumeragi::NewStub(channel)) {}

  // TODO: More clear method name. 'verify' should be used only for checking valid data (assertion).
  // CallBack
  ::iroha::Response* Verify(
      const std::unique_ptr<ConsensusEvent>& consensusEvent) const {
    ClientContext context;
    flatbuffers::BufferRef<Response> response;
    logger::info("connection") << "Operation";
    logger::info("connection")
        << "size: " << consensusEvent->peerSignatures()->size();
    logger::info("connection")
        << "CommandType: "
        << consensusEvent->transactions()->Get(0)->command_type();

    // For now, ConsensusEvent has one transaction.
    auto transaction = consensusEvent->transactions()->Get(0);

    flatbuffers::FlatBufferBuilder fbb;

    std::vector<flatbuffers::Offset<Signature>> signatures;
    std::vector<flatbuffers::Offset<Transaction>> transactions;
    std::vector<uint8_t> _hash(*transaction->hash()->begin(),
                               *transaction->hash()->end());
    std::vector<uint8_t> data(*transaction->attachment()->data()->begin(),
                              *transaction->attachment()->data()->end());

    ::iroha::CreateAttachmentDirect(
        fbb, transaction->attachment()->mime()->c_str(), &data);

    std::vector<flatbuffers::Offset<::iroha::Signature>> tx_signatures;

    for (auto&& txSig : *transaction->signatures()) {
      std::vector<uint8_t> _data;
      for (auto d : *txSig->signature()) {
        _data.emplace_back(d);
      }
      tx_signatures.emplace_back(::iroha::CreateSignatureDirect(
          fbb, txSig->publicKey()->c_str(), &_data));
    }

    transactions.emplace_back(::iroha::CreateTransactionDirect(
        fbb, transaction->creatorPubKey()->c_str(),
        transaction
            ->command_type(),  // confusing name, transactions / transaction
        reinterpret_cast<flatbuffers::Offset<void>*>(
            const_cast<void*>(transaction->command())),
        &tx_signatures, &_hash,
        ::iroha::CreateAttachmentDirect(
            fbb, transaction->attachment()->mime()->c_str(), &data)));

    // Create ConsensusEvent
    auto event_buf =
        ::iroha::CreateConsensusEventDirect(fbb, &signatures, &transactions);
    fbb.Finish(event_buf);

    auto req_consensusEvent = flatbuffers::BufferRef<::iroha::ConsensusEvent>(
        fbb.GetBufferPointer(), fbb.GetSize());
    Status status = stub_->Verify(&context, req_consensusEvent, &response);

    if (status.ok()) {
      logger::info("connection")
          << "response: " << response.GetRoot()->message();
      return response.GetRoot();
    } else {
      logger::error("connection")
          << status.error_code() << ": " << status.error_message();
      return response.GetRoot();
      // std::cout << status.error_code() << ": " << status.error_message();
      // return {"RPC failed", RESPONSE_ERRCONN};
    }
  }

  // TODO: Make difference clear between argument response and return value response.
  ::iroha::Response* Torii(
      const std::unique_ptr<Transaction>& transaction) const {
    flatbuffers::BufferRef<Response> response;
    ClientContext context;

    flatbuffers::FlatBufferBuilder fbb;

    std::vector<flatbuffers::Offset<Signature>> signatures; // TODO: Specify who's signature.
    std::vector<uint8_t> _hash(*transaction->hash()->begin(),
                               *transaction->hash()->end());
    std::vector<uint8_t> data(*transaction->attachment()->data()->begin(),
                              *transaction->attachment()->data()->end());

    ::iroha::CreateAttachmentDirect(
        fbb, transaction->attachment()->mime()->c_str(), &data);

    std::vector<flatbuffers::Offset<Signature>> tx_signatures;

    for (auto&& txSig : *transaction->signatures()) {
      std::vector<uint8_t> _data;
      for (auto d : *txSig->signature()) {
        _data.emplace_back(d);
      }
      tx_signatures.emplace_back(::iroha::CreateSignatureDirect(
          fbb, txSig->publicKey()->c_str(), &_data));
    }

    auto tx = ::iroha::CreateTransactionDirect(
        fbb, transaction->creatorPubKey()->c_str(),
        transaction
            ->command_type(),  // confusing name, transactions / transaction
        reinterpret_cast<flatbuffers::Offset<void>*>(
            const_cast<void*>(transaction->command())),
        &tx_signatures, &_hash,
        ::iroha::CreateAttachmentDirect(
            fbb, transaction->attachment()->mime()->c_str(), &data));

    fbb.Finish(tx);
    auto req_transaction = flatbuffers::BufferRef<::iroha::Transaction>(
        fbb.GetBufferPointer(), fbb.GetSize());

    Status status = stub_->Torii(&context, req_transaction, &response);

    if (status.ok()) {
      logger::info("connection")
          << "response: " << response.GetRoot()->message();
      return response.GetRoot();
    } else {
      logger::error("connection")
          << status.error_code() << ": " << status.error_message();
      // std::cout << status.error_code() << ": " << status.error_message();
      return response.GetRoot();
    }
  }

 private:
  std::unique_ptr<Sumeragi::Stub> stub_;
};

}  // namespace connection

#endif
