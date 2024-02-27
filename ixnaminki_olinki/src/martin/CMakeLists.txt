cmake_minimum_required(VERSION 3.8)
project(martin)


# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(structuras REQUIRED)
find_package(std_srvs REQUIRED)

add_executable(server src/martino_service.cpp)
target_link_libraries(server pigpiod_if2)
ament_target_dependencies(server rclcpp structuras)

add_executable(client src/martino_client.cpp)
ament_target_dependencies(client rclcpp structuras)



install(TARGETS
    server
    client
  DESTINATION lib/${PROJECT_NAME})


ament_package()
