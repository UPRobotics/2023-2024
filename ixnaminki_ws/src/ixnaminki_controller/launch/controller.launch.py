import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import Command
from ament_index_python import get_package_share_directory

def generate_launch_description():
    robot_description = ParameterValue(
        Command(
            [
                "xacro ",
                os.path.join(get_package_share_directory("ixnaminki_description"), "urdf", "ixnaminki_des.urdf.xacro"),
            ]
        ),
        value_type=str,
    )
    robot_state_publisher_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": robot_description}],
    )
    joint_state_broadcaster_spawner = Node(
        package="controller_manager", 
        executable="spawner",
        arguments=[
            "joint_state_broadcaster",
            "--controller-manager",
            "/controller_manager",
        ]
    )
    arm_controller_spawner = Node(
        package="controller_manager", 
        executable="spawner",
        arguments=[
            "arm_controller",
            "--controller-manager",
            "/controller_manager",
        ]
    )
    flipper01_controller_spawner = Node(
        package="controller_manager", 
        executable="spawner",
        arguments=[
            "flipper01_controller",
            "--controller-manager",
            "/controller_manager",
        ]
    )
    flipper02_controller_spawner = Node(
        package="controller_manager", 
        executable="spawner",
        arguments=[
            "flipper02_controller",
            "--controller-manager",
            "/controller_manager",
        ]
    )
    return LaunchDescription(
        [
            robot_state_publisher_node,
            joint_state_broadcaster_spawner,
            arm_controller_spawner,
            flipper01_controller_spawner,
            flipper02_controller_spawner,
        ]
    )

