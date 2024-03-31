#include "lite_tcp.hpp"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

class ServerTest : public testing::Test {
protected:
  ServerTest() {}
  //	~ServerTest() override {}
  //	void Setup() override {}
  void TearDown() override { server.stop(); }
  LiteTcpServer server;
};
TEST_F(ServerTest, basicop) {
  EXPECT_EQ(server.run(), true);
  EXPECT_EQ(server.send("OK", strlen("OK")), false);
  EXPECT_EQ(server.running(), true);
  LiteTcpClient client;
  EXPECT_EQ(client.run(), true);
  EXPECT_EQ(client.running(), true);
  EXPECT_EQ(server.send("Hi from server", strlen("Hi from server")), false);
  EXPECT_EQ(client.send("OK", strlen("OK")), true);
  // Only after server receive data, it can send data to client
  while (!server.ready()) {
    usleep(10000);
  }
  // now server is ready to send data
  EXPECT_EQ(server.send("Hi from server", strlen("Hi from server")), true);
  client.stop();
}