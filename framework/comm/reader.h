#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <queue>

#include "common/common.h"

namespace frad {
namespace framework {

template <class M>
class Reader {
 public:
  Reader() = default;
  virtual ~Reader() = default;

  std::optional<M> Read();
  std::queue<M> ReadAll();

  void Enque(M&& message);

 private:
  std::queue<M> messages_;
  std::mutex mutex_;

  DISALLOW_COPY_MOVE_ASSIGN(Reader);
};

template <class M>
std::optional<M> Reader<M>::Read() {
  std::unique_lock lock(mutex_);
  if (messages_.empty()) {
    return std::nullopt;
  }

  M mesage{std::move(messages_.front())};
  messages_.pop();
  return std::move(mesage);
}

template <class M>
std::queue<M> Reader<M>::ReadAll() {
  std::unique_lock lock(mutex_);
  return std::move(messages_);
}

template <class M>
void Reader<M>::Enque(M&& message) {
  std::unique_lock lock(mutex_);
  messages_.emplace(std::move(message));
}

}  // namespace framework
}  // namespace frad
