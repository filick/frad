#include "framework/comm/channel.h"

#include <gtest/gtest.h>

#include <chrono>
#include <string>
#include <thread>
#include <vector>

namespace frad {
namespace framework {

TEST(ChannelTest, SingletonBehavior) {
  auto& channel1 = Channel<int>::GetInstance("c1");
  EXPECT_EQ(channel1.topic(), "c1");

  auto& channel2 = Channel<int>::GetInstance("c1");
  EXPECT_EQ(&channel1, &channel2);

  auto& channel3 = Channel<int>::GetInstance("c3");
  EXPECT_NE(&channel1, &channel3);

  auto& channel4 = Channel<double>::GetInstance("c1");
  EXPECT_NE(reinterpret_cast<void*>(&channel1), reinterpret_cast<void*>(&channel4));
}

TEST(ChannelTest, ThreadSafty) {
  std::vector<std::thread> threads;
  for (int i = 0; i < 100; ++i) {
    threads.emplace_back(
        [](const std::string& topic) {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          EXPECT_EQ(&Channel<int>::GetInstance(topic), &Channel<int>::GetInstance(topic));
        },
        std::to_string(i % 20));
  }
  for (auto& thread : threads) {
    thread.join();
  }
}

}  // namespace framework
}  // namespace frad
