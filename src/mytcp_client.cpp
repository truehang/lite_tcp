// Copyright 2023 Better Tomorrow.
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

#include <iostream>
#include <memory>
#include <vector>

#include "lite_tcp.hpp"
#include <cstring>
#include <string>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Usage: ./mytcp_client <server_ip> <server_port>\n";
    return 0;
  }
  int pid = getpid();
  std::string str("hello from client pid " + std::to_string(pid));
  auto tcp_client = std::make_shared<LiteTcpClient>(
      argv[1], std::stoi(argv[2]), [](char *data, size_t size) {
        (void)data;
        std::cout << "from server size: " << size << ", data: " << data
                  << std::endl;
      });
  tcp_client->run();

  int cnt = 0;
  while (tcp_client->running()) {
    auto tmp = str + ", cnt " + std::to_string(++cnt);
    std::vector<char> buf(1000); // 1000
    memcpy(buf.data(), tmp.data(), tmp.size());
    tcp_client->send(buf.data(), buf.size());
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(20ms);
  }

  return 0;
}
