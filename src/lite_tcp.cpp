#include "lite_tcp.hpp"

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <fcntl.h>
#include <sys/epoll.h>

namespace matrix::drivers::sensor_adapter
{
RecvCallback DefaultRecvCallback{[](char * data, size_t size) {
    (void)data;
    std::cout << "recv from remote, size: " << size << std::endl;
  }};

LiteTcpServer::LiteTcpServer(const std::string & server_ip, int port, const RecvCallback & func)
: sockfd_(-1), run_(false), stop_(false), callback_(func)
{
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  // keep alive
  int keep_alive = 1;
  if(setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &keep_alive, sizeof(keep_alive)) < 0)
  {
    perror("setsockopt keep_alive");
    exit(EXIT_FAILURE);
  }
  // reuse address
  int reuse_address = 1;
  if(setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address)) < 0)
  {
    perror("setsockopt reuse_address");
    exit(EXIT_FAILURE);
  }
  // reuse port
  int reuse_port = 1;
  if(setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &reuse_port, sizeof(reuse_port)) < 0)
  {
    perror("setsockopt reuse_address");
    exit(EXIT_FAILURE);
  }

  // set nonblocking
  int sock_flg = fcntl(sockfd_, F_GETFL, 0);
  if(sock_flg < 0)
  {
    perror("fcntl F_GETFL");
    exit(EXIT_FAILURE);
  }
  if(fcntl(sockfd_, F_SETFL, sock_flg | O_NONBLOCK) < 0)
  {
    perror("fcntl O_NONBLOCK");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in ser;
  bzero(&ser, sizeof(ser));
  ser.sin_family = AF_INET;
  ser.sin_addr.s_addr = inet_addr(server_ip.c_str());   // 0.0.0.0
  ser.sin_port = htons(port);

  if (bind(sockfd_, reinterpret_cast<sockaddr *>(&ser), sizeof(ser)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
}

LiteTcpServer::~LiteTcpServer()
{
  if (run_ || stop_) {
    process_thread_.join();
  }
  // closing the connected socket
  for(auto conn_fd : conn_fds_)
    close(conn_fd);
  // closing the listening socket
  close(sockfd_);
}

bool LiteTcpServer::run()
{
  if (listen(sockfd_, 10) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  if (sockfd_ < 0) {
    return false;
  }

  run_ = true;
  process_thread_ = std::thread(
    [&] {
      
      int efd = epoll_create1(0);
      if(efd == -1)
      {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
      }

      struct epoll_event ev;
      ev.events = EPOLLIN | EPOLLET; // The associated file is available for read operations.
      ev.data.fd = sockfd_;
      if(epoll_ctl(efd, EPOLL_CTL_ADD, sockfd_, &ev) == -1)
      {
        perror("epoll_ctl: listen_sock");
        close(sockfd_);
        exit(EXIT_FAILURE);
      }
      #define MAX_EVENTS 10
      struct epoll_event events[MAX_EVENTS];
      std::vector<char> buf;
      buf.resize(1 << 20); // 1 M
      while(run_)
      {
        // 1000ms time out
        int nfds = epoll_wait(efd, events, MAX_EVENTS, 1000);
        if(nfds == -1)
        {
          // time out
          if(errno == EINTR)
            continue;
          else{
            perror("epoll_wait");
            exit(EXIT_FAILURE);
          }
        }
        
        for(int i = 0; i < nfds; ++i)
        {
          if(events[i].data.fd == sockfd_)
          {
            struct sockaddr_in cli;
            socklen_t len = sizeof(cli);
            bzero(&cli, sizeof(cli));
            // nonblock 
            int conn_fd =  accept4(sockfd_, reinterpret_cast<sockaddr *>(&cli), &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
            if(conn_fd == -1)
            {
              perror("accept");
              exit(EXIT_FAILURE);
            }
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = conn_fd;
            if(epoll_ctl(efd, EPOLL_CTL_ADD, conn_fd, &ev) == -1)
            {
              perror("epoll_ctl: add conn_sock");
              exit(EXIT_FAILURE);
            }
            std::cout << "connected to client, " <<  inet_ntoa(cli.sin_addr) << ":"<< ntohs(cli.sin_port) << std::endl;
            conn_fds_.insert(conn_fd);
          } else{
            int fd = events[i].data.fd; 
            ssize_t size = read(fd, buf.data(), buf.size());
            if (size > 0)
            {
              if(run_)
              {
                callback_(buf.data(), size);
                bzero(buf.data(), size);
              }
            } else
            {
              ev.events = EPOLLIN | EPOLLET;
              ev.data.fd = fd;
              if(epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev) == -1)
              {
                perror("epoll_ctl: delete conn_sock");
              }
              close(fd);
              if(conn_fds_.count(fd)) conn_fds_.erase(fd);
            }
          }
        }
      }
    });
  return true;
}

bool LiteTcpServer::send(BufType send_data)
{
  if (conn_fds_.empty()) {
    return false;
  }
  for(auto conn_fd : conn_fds_)
  if (write(conn_fd, send_data.data(), send_data.size()) < 0) {
    perror("write for");
    return false;
  }
  return true;
}

bool LiteTcpServer::send(char *data, size_t size){
  if (conn_fds_.empty()) {
    return false;
  }
  for(auto conn_fd : conn_fds_)
  if (write(conn_fd, data, size) < 0) {
    perror("write");
    return false;
  }
  return true;
}

void LiteTcpServer::stop()
{
    run_ = false;
    stop_ = true;
}

// Client
LiteTcpClient::LiteTcpClient(
  const std::string & server_ip, int server_port,
  const RecvCallback & func)
: sockfd_(-1), run_(false), stop_(false), callback_(func)
{
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in cli;
  bzero(&cli, sizeof(cli));
  cli.sin_family = AF_INET;
  cli.sin_addr.s_addr = inet_addr(server_ip.c_str());  // "127.0.0.1"
  cli.sin_port = htons(server_port);

  if (connect(sockfd_, reinterpret_cast<sockaddr *>(&cli), sizeof(cli)) < 0) {
    perror("connect");
    std::cout << "Server not ready\n";
  } else {
    std::cout << "Connected to server\n";
  }
}

LiteTcpClient::~LiteTcpClient()
{
  if (run_ || stop_) {
    process_thread_.join();
  }
  close(sockfd_);
}

bool LiteTcpClient::run()
{
  if (sockfd_ < 0) {
    return false;
  }
  run_ = true;
  process_thread_ = std::thread(
    [this] {
      std::vector<char> buf;
      buf.resize(1 << 20); // 1 M
      while (run_) {
        ssize_t size = read(sockfd_, buf.data(), buf.size());
        if (size > 0) {
          callback_(buf.data(), size);
          bzero(buf.data(), size);
        } else if (size == 0) {
          std::cout << "server is shutdown" << std::endl;
          break;
        } else {
          if(errno != EINTR)
          {
            perror("read");
          }else break; // stop
        }
      }
    });
  return true;
}

bool LiteTcpClient::send(BufType send_data)
{
  if (write(sockfd_, send_data.data(), send_data.size()) < 0) {
    std::cout << "write error" << std::endl;
    return false;
  }
  return true;
}

bool LiteTcpClient::send(char *data, size_t size){
  if (write(sockfd_, data, size) < 0) {
    std::cout << "write error" << std::endl;
    return false;
  }
  return true;
}

void LiteTcpClient::stop()
{
    run_ = false;
    stop_ = true;
}

}  // namespace matrix::drivers::sensor_adapter
