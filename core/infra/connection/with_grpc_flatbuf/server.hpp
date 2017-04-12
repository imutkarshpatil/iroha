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

#ifndef CORE_CONNECTION_WITH_GRPC_FLATBUF_CLIENT_SERVER_HPP
#define CORE_CONNECTION_WITH_GRPC_FLATBUF_CLIENT_SERVER_HPP

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
    auto txPtr = std::make_unique<Transaction>(std::move(*txRawPtr));
    txRawPtr = nullptr;

    // Extract signature
    auto signatures = txPtr->signatures();  // TODO: sign(hash + timestamp) ?
                                            // Where is timestamp?

    // Push new processTransaction() task to pool
    iroha::SumeragiImpl::Torii::scheduledTask.call(
        "", std::move(*txPtr));  // currently, from = "" (dummy)

    // Create response
    fbb.Clear();

    auto resOffset = ::iroha::CreateResponseDirect(
        fbb,
        /* message    */ "OK",
        /* iroha::Code*/ iroha::Code_COMMIT,
        /* signature  */ signatures->Get(0));  // TODO

    fbb.Finish(resOffset);

    // Since we keep reusing the same FlatBufferBuilder, the memory it owns
    // remains valid until the next call (this BufferRef doesn't own the
    // memory it points to).

    // Only valid until next transaction is received.
    *response = flatbuffers::BufferRef<::iroha::Response>(
        fbb.GetBufferPointer(), fbb.GetSize());

    return grpc::Status::OK;
  }

 private:
  flatbuffers::FlatBufferBuilder fbb;
};

}  // namespace connection

#endif
