cmake_minimum_required(VERSION 3.8)
project(ixnaminki2_remote)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(rclpy REQUIRED)
find_package(std_srvs)
find_package(posicion)
find_package(control_msgs)


add_executable(tasko_server src/task_server.cpp)
ament_target_dependencies(tasko_server rclcpp std_srvs)
ament_target_dependencies(tasko_server rclcpp rclcpp_components)
ament_target_dependencies(tasko_server rclcpp moveit_ros_planning_interface)


add_executable(tasko_client src/task_client.cpp)
ament_target_dependencies(tasko_client rclcpp std_srvs)



add_executable(plan_server src/plan_server.cpp)
ament_target_dependencies(plan_server rclcpp posicion)
ament_target_dependencies(plan_server rclcpp rclcpp_components)
ament_target_dependencies(plan_server rclcpp moveit_ros_planning_interface)

add_executable(plan_client src/plan_client.cpp)
ament_target_dependencies(plan_client rclcpp posicion)


add_executable(prueba src/prueba.cpp)
ament_target_dependencies(prueba rclcpp control_msgs)


install (TARGETS
  tasko_server
  tasko_client
  plan_server
  plan_client
  prueba
  DESTINATION lib/${PROJECT_NAME}
)

install(
   DIRECTORY launch 
   DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
