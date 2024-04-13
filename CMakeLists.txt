cmake_minimum_required(VERSION 3.8)
project(
lite_tcp
DESCRIPTION "A robust and simple tcp server can handles multiple tcp clients's communications."
VERSION 0.1.21
)

option(ENABLE_CLANG_TIDY "Run clang-tidy" OFF)
if(ENABLE_CLANG_TIDY)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
#Default to C++ 17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-Wfatal-errors") # stop at first error

set(OUR_TARGETS)
include_directories(include)
add_library(lite_tcp SHARED src/lite_tcp.cpp )
target_link_libraries(lite_tcp PRIVATE pthread)
list(APPEND OUR_TARGETS lite_tcp)

add_executable(mytcp_server src/mytcp_server.cpp )
target_link_libraries(mytcp_server lite_tcp)
list(APPEND OUR_TARGETS mytcp_server)

add_executable(mytcp_client src/mytcp_client.cpp)
target_link_libraries(mytcp_client lite_tcp)
list(APPEND OUR_TARGETS mytcp_client)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
# cache our download
#  SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/googletest
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# Silence clang-tidy warnings from googletest
set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")

enable_testing()
add_executable(lite_tcp_test test/lite_tcp_test.cc)
target_link_libraries(lite_tcp_test GTest::gtest_main lite_tcp)

include(GoogleTest)
gtest_discover_tests(lite_tcp_test)

set(CPACK_COMPONENTS_ALL
    lite_tcp
)

install(TARGETS
  ${OUR_TARGETS}
  ARCHIVE DESTINATION lib
  COMPONENT lite_tcp
  LIBRARY DESTINATION lib
  COMPONENT lite_tcp
  RUNTIME DESTINATION bin
  COMPONENT lite_tcp
)

install(DIRECTORY include/
  DESTINATION include/ 
  COMPONENT lite_tcp)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Packing)
