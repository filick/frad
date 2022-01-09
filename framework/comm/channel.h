#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <utility>

#include "common/common.h"
#include "framework/comm/reader.h"
#include "framework/comm/writer.h"

namespace frad {
namespace framework {

template <class M>
class Channel {
 public:
  static Channel<M>& GetInstance(const std::string& topic);
  // TODO(fuwang): unable to make this constructor private because std::map needs to access to it. Find a better
  // solution later.
  explicit Channel(const std::string& topic) : topic_(topic) {}
  virtual ~Channel() = default;

  const std::string& topic() const { return topic_; }
  void Register(std::shared_ptr<Writer<M>>& writer);
  void Register(std::shared_ptr<Reader<M>>& reader);

 private:
  void DeliverMessage(M&& mesage);

  const std::string topic_;
  std::list<std::weak_ptr<Reader<M>>> readers_;
  std::mutex mutex_;

  DISALLOW_COPY_MOVE_ASSIGN(Channel<M>);
};

template <class M>
Channel<M>& Channel<M>::GetInstance(const std::string& topic) {
  static std::map<std::string, Channel<M>> all_channels_;
  static std::shared_mutex global_mutex_;
  {
    std::shared_lock lock(global_mutex_);
    if (all_channels_.count(topic)) {
      return all_channels_.at(topic);
    }
  }
  {
    std::unique_lock lock(global_mutex_);
    if (all_channels_.count(topic) == 0) {
      all_channels_.emplace(topic, topic);
    }
    return all_channels_.at(topic);
  }
}

template <class M>
void Channel<M>::Register(std::shared_ptr<Writer<M>>& writer) {
  writer->RegisterCallback(std::bind(&Channel<M>::DeliverMessage, this, std::placeholders::_1));
}

template <class M>
void Channel<M>::Register(std::shared_ptr<Reader<M>>& reader) {
  std::lock_guard<std::mutex> lock(mutex_);
  readers_.emplace_back(reader);
}

template <class M>
void Channel<M>::DeliverMessage(M&& mesage) {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto iter = readers_.begin(); iter != readers_.end();) {
    if (std::shared_ptr<Reader<M>> reader = iter->lock()) {
      reader->Enque(M(mesage));
      ++iter;
    } else {
      iter = readers_.erase(iter);
    }
  }
}

}  // namespace framework
}  // namespace frad
