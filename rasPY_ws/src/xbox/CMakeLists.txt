cmake_minimum_required(VERSION 3.8)
project(xbox)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(formatosa)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs)
find_package(rcl_interfaces  REQUIRED)


add_executable(xbox_reader src/joystick_reader.cpp)
ament_target_dependencies(xbox_reader rclcpp formatosa)
ament_target_dependencies(xbox_reader rclcpp std_srvs)
ament_target_dependencies(xbox_reader rclcpp sensor_msgs)
ament_target_dependencies(xbox_reader rclcpp rcl_interfaces)

install(TARGETS
  xbox_reader
  DESTINATION lib/${PROJECT_NAME}

)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
