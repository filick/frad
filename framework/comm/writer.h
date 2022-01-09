#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "common/common.h"

namespace frad {
namespace framework {

template <class M>
class Writer {
 public:
  using Callback = std::function<void(M&&)>;

  Writer() = default;
  virtual ~Writer() = default;

  void Write(M&& message);
  void RegisterCallback(const Callback& func);

 private:
  Callback func_;

  DISALLOW_COPY_MOVE_ASSIGN(Writer);
};

template <class M>
void Writer<M>::Write(M&& message) {
  if (func_) {
    func_(std::move(message));
  }
}

template <class M>
void Writer<M>::RegisterCallback(const Callback& func) {
  func_ = func;
}

}  // namespace framework
}  // namespace frad
