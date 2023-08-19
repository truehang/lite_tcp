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

#ifndef LITE_TCP_HPP_
#define LITE_TCP_HPP_

#include <functional>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

using BufType = const std::vector<char> &;
using RecvCallback = std::function<void(char *, size_t)>;
extern RecvCallback DefaultRecvCallback;
// TCP最大报文长度1460字节
class LiteTcpServer {
public:
  LiteTcpServer(const std::string &server_ip = "0.0.0.0", int port = 9999,
                const RecvCallback &func = DefaultRecvCallback);
  ~LiteTcpServer();
  bool run();
  bool send(BufType send_data);
  bool send(char *data, size_t size);
  void stop();
  bool running() { return run_; }
  bool ready() { return running() && !conn_fds_.empty(); }

private:
  int sockfd_;
  std::unordered_set<int> conn_fds_;
  bool run_;
  bool stop_;
  std::thread process_thread_;
  RecvCallback callback_;
};

class LiteTcpClient {
public:
  LiteTcpClient(const std::string &server_ip = "127.0.0.1",
                int server_port = 9999,
                const RecvCallback &func = DefaultRecvCallback);
  ~LiteTcpClient();
  bool run();
  bool send(BufType send_data);
  bool send(char *data, size_t size);
  void stop();
  bool running() { return run_; }

private:
  int sockfd_;
  bool run_;
  bool stop_;
  std::thread process_thread_;
  RecvCallback callback_;
};
#endif // LITE_TCP_HPP_
