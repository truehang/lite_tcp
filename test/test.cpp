// Copyright (c) 2021 Geekplus Inc
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>

#include <memory>

#include "lite_tcp.hpp"
#include <vector>
#include <chrono>
#include <thread>
using matrix::drivers::sensor_adapter::LiteTcpClient;
using matrix::drivers::sensor_adapter::LiteTcpServer;

TEST(lite_tcp_test, server) {
  auto tcp_server = std::make_shared<LiteTcpServer>();
}

TEST(lite_tcp_test, client) {
  auto tcp_client = std::make_shared<LiteTcpClient>();
}

TEST(lite_tcp_test, server_send)
{
  auto tcp_server = std::make_shared<LiteTcpServer>("127.0.0.1", 1779);
  EXPECT_TRUE(tcp_server->run());
  auto tcp_client = std::make_shared<LiteTcpClient>("127.0.0.1", 1779);
  EXPECT_TRUE(tcp_client->run());
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(10ms);
  std::vector<char> buf(10, 'a');
  EXPECT_TRUE(tcp_server->send(buf));
  std::this_thread::sleep_for(10ms);
  tcp_client->stop();
  EXPECT_TRUE(tcp_server->send(buf));
  tcp_server->stop();
  EXPECT_TRUE(tcp_client->send(buf));
}


int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
