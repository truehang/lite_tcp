#include <iostream>
#include <memory>

#include "lite_tcp.hpp"
#include <csignal>
#include <cstring>

using matrix::drivers::sensor_adapter::BufType;
using matrix::drivers::sensor_adapter::LiteTcpServer;
volatile bool gRunStatus = true;
void signal_handler(int signal)
{
  if(signal == SIGINT)
  {
    gRunStatus = false;
  }
}

int main()
{
  // Install a signal handler
  std::signal(SIGINT, signal_handler);

  auto tcp_server = std::make_shared<LiteTcpServer>(
    "0.0.0.0", 9999, [](char *data, size_t size) {

      std::cout << "recv from client, size: " << size << std::endl;
      for(size_t i = 0; i < size; i++)
      {
        if(*(static_cast<char *>(data) + i) != '\0')
        {
          std::cout << "at pos " << i << ", string: " << static_cast<char *>(data) + i << std::endl;
          break;
        }
      }
    });
  tcp_server->run();
  
  int cnt = 1;
  while(gRunStatus)
  {
    auto tmp = "cnt " + std::to_string(cnt);
    std::vector<char> buf(1000);
    memcpy(buf.data(), tmp.data(), tmp.size());
    if(tcp_server->send(buf.data(), buf.size()))
    {
      cnt++;
    }
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(20ms);
  }
  tcp_server->stop();
  return 0;
}
