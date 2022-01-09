#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include "common/common.h"
#include "framework/comm/channel.h"
#include "framework/comm/reader.h"
#include "framework/comm/writer.h"

namespace frad {
namespace framework {

class Comm {
 public:
  template <class M>
  static std::shared_ptr<Writer<M>> CreateWriter(const std::string& topic);

  template <class M>
  static std::shared_ptr<Reader<M>> CreateReader(const std::string& topic);

 private:
  Comm() = default;
  virtual ~Comm() = default;

  DISALLOW_COPY_MOVE_ASSIGN(Comm);
};

template <class M>
std::shared_ptr<Writer<M>> Comm::CreateWriter(const std::string& topic) {
  auto writer = std::make_shared<Writer<M>>();
  auto& channel = Channel<M>::GetInstance(topic);
  channel.Register(writer);
  return std::move(writer);
}

template <class M>
std::shared_ptr<Reader<M>> Comm::CreateReader(const std::string& topic) {
  auto reader = std::make_shared<Reader<M>>();
  auto& channel = Channel<M>::GetInstance(topic);
  channel.Register(reader);
  return std::move(reader);
}

}  // namespace framework
}  // namespace frad
