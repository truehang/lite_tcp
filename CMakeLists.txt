cmake_minimum_required(VERSION 3.8)
project(lite_tcp)

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


install(TARGETS
  lite_tcp
  mytcp_server
  mytcp_client 
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include/)

