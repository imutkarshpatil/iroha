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

#ifndef SCHEDULED_TASK_TO_CONSENSUS_HPP
#define SCHEDULED_TASK_TO_CONSENSUS_HPP

#include <util/exception.hpp>
#include <memory>

/**
 * @brief Store scheduled consensus to take.
 * @details Multiple setting task is not assumed. First, set() task. Then call() task.
 * 
 * @tparam CallBackFunc Function to take consensus. Consensus event is bound on the function.
 */
template <class CallBackFunc>
class ScheduledTaskToConsensus {
 public:
  void set(CallBackFunc&& rhs) {
    if (receiver_) {
      throw exception::DuplicateSetException(
          "ScheduledConsensus<" + std::string(typeid(CallBackFunc).name()) + ">",
          __FILE__);
    }
    task_ = std::make_shared<CallBackFunc>(rhs);
  }

  template <typename... Args>
  void call(Args&&... args) {
    if (!receiver_) {
      throw exception::UnsetException(
        "ScheduledProcess<" + std::string(typeid(CallBackFunc).name()) + ">",
        __FILE__
        );
    }
    (*task_)(std::forward<Args>(args)...);
    task_.reset();
  }

 private:
  std::shared_ptr<CallBackFunc> task_;
};

#endif
