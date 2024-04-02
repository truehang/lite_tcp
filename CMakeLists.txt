cmake_minimum_required(VERSION 3.8)
project(
lite_tcp
DESCRIPTION "A robust and simple tcp server can handles multiple tcp clients's communications."
VERSION 0.1.20
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
#Default to C++ 17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-Wfatal-errors") # stop at first error

include_directories(include)
add_library(lite_tcp src/lite_tcp.cpp )
target_link_libraries(lite_tcp pthread)

add_executable(mytcp_server src/mytcp_server.cpp )
target_link_libraries(mytcp_server lite_tcp)

add_executable(mytcp_client src/mytcp_client.cpp)
target_link_libraries(mytcp_client lite_tcp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(lite_tcp_test test/lite_tcp_test.cc)
target_link_libraries(lite_tcp_test GTest::gtest_main lite_tcp)

include(GoogleTest)
gtest_discover_tests(lite_tcp_test)

set(CPACK_COMPONENTS_ALL
    lite_tcp
)

install(TARGETS
  lite_tcp
  mytcp_server
  mytcp_client 
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  COMPONENT lite_tcp
)

install(DIRECTORY include/
  DESTINATION include/ 
  COMPONENT lite_tcp)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Packing)
