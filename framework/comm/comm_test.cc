#include "framework/comm/comm.h"

#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>

namespace frad {
namespace framework {

TEST(CommTest, BasicCommunication) {
  std::shared_ptr<Writer<int>> writer = Comm::CreateWriter<int>("int_channel");
  std::shared_ptr<Reader<int>> reader = Comm::CreateReader<int>("int_channel");

  writer->Write(1);
  std::optional<int> message = reader->Read();
  EXPECT_TRUE(message);
  EXPECT_EQ(*message, 1);
  EXPECT_TRUE(reader->ReadAll().empty());
  EXPECT_FALSE(reader->Read());

  writer->Write(1);
  writer->Write(2);
  std::queue<int> messages = reader->ReadAll();
  EXPECT_EQ(messages.size(), 2);
  EXPECT_EQ(messages.front(), 1);
  EXPECT_EQ(messages.back(), 2);
  EXPECT_TRUE(reader->ReadAll().empty());
}

TEST(CommTest, Multi2Multi) {
  using ConstIntPtr = std::shared_ptr<const int>;
  std::shared_ptr<Writer<ConstIntPtr>> writer1 = Comm::CreateWriter<ConstIntPtr>("int_channel");
  std::shared_ptr<Writer<ConstIntPtr>> writer2 = Comm::CreateWriter<ConstIntPtr>("int_channel");
  std::shared_ptr<Reader<ConstIntPtr>> reader1 = Comm::CreateReader<ConstIntPtr>("int_channel");
  std::shared_ptr<Reader<ConstIntPtr>> reader2 = Comm::CreateReader<ConstIntPtr>("int_channel");

  writer1->Write(std::make_shared<const int>(1));
  writer2->Write(std::make_shared<const int>(2));
  std::optional<ConstIntPtr> m11 = reader1->Read();
  std::optional<ConstIntPtr> m12 = reader1->Read();
  std::optional<ConstIntPtr> m21 = reader2->Read();
  std::optional<ConstIntPtr> m22 = reader2->Read();
  EXPECT_TRUE(m11);
  EXPECT_EQ(*(*m11), 1);
  EXPECT_TRUE(m12);
  EXPECT_EQ(*(*m12), 2);
  EXPECT_TRUE(m21);
  EXPECT_EQ(*(*m21), 1);
  EXPECT_TRUE(m22);
  EXPECT_EQ(*(*m22), 2);

  EXPECT_EQ(m11->get(), m21->get());
  EXPECT_EQ(m12->get(), m12->get());
  EXPECT_EQ(m11->use_count(), 2);
  EXPECT_EQ(m21->use_count(), 2);
  EXPECT_EQ(m12->use_count(), 2);
  EXPECT_EQ(m22->use_count(), 2);
}

}  // namespace framework
}  // namespace frad
