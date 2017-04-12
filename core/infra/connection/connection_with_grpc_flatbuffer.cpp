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

#include <util/logger.hpp>
#include "with_grpc_flatbuf/client.hpp"
#include "with_grpc_flatbuf/server.hpp"

namespace connection {

std::mutex wait_for_server;
ServerBuilder builder;
grpc::Server* server = nullptr;
std::condition_variable server_cv;

void initialize_peer() {
  // ToDo catch exception of to_string


  logger::info("Connection GRPC") << " initialize_peer ";
}

int run() {
  logger::info("Connection GRPC") << " RUN ";

  auto address =
      "0.0.0.0:" +
      std::to_string(
          config::IrohaConfigManager::getInstance().getGrpcPortNumber(50051));

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
}  // namespace connection
