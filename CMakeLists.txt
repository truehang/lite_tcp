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

# find dependencies
#find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
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

#ament_export_include_directories(include)
#ament_export_libraries(lite_tcp)

  if(BUILD_TESTING)
	  #  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  #ament_lint_auto_find_test_dependencies()

  #find_package(ament_cmake_gtest REQUIRED)
  #ament_add_gtest(${PROJECT_NAME}_unit_test test/test.cpp)
  #target_link_libraries(${PROJECT_NAME}_unit_test lite_tcp)
endif()

#ament_package()
