import os
from os import pathsep
from ament_index_python.packages import get_package_share_directory, get_package_prefix

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, SetEnvironmentVariable, IncludeLaunchDescription
from launch.substitutions import Command, LaunchConfiguration

from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    ixnaminki_description_dir = get_package_share_directory('ixnaminki2_description')
    ixnaminki_description_share = os.path.join(get_package_prefix('ixnaminki2_description'), 'share')
    gazebo_ros_dir = get_package_share_directory('gazebo_ros')

    model_path = os.path.join(ixnaminki_description_dir, "models")
    model_path += pathsep + os.path.join(ixnaminki_description_share)
    robot_description = ParameterValue(Command(['xacro ', LaunchConfiguration('model')]),
                                       value_type=str)
    
    model_arg = DeclareLaunchArgument(name='model', default_value=os.path.join(
        ixnaminki_description_dir, 'urdf', 'brazo.urdf.xacro'
        )
    )

    env_variable = SetEnvironmentVariable("GAZEBO_MODEL_PATH", model_path)

    robot_description = ParameterValue(Command(['xacro ', LaunchConfiguration('model')]),
                                       value_type=str)

    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[
            {'robot_description': robot_description},         
            {'use_sim_time': True},
        ]
    )

    start_gazebo_server = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_ros_dir, 'launch', 'gzserver.launch.py')
        )
    )

    start_gazebo_client = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_ros_dir, 'launch', 'gzclient.launch.py')
        )
    )

    spawn_robot = Node(
        package='gazebo_ros', 
        executable='spawn_entity.py',
        arguments=['-entity', 'ixnaminki_olinki_arm',
                    '-topic', 'robot_description',
                    ],
        output='screen',
        parameters=[
            {'use_sim_time': True},
        ]
    )

    return LaunchDescription([
        env_variable,
        model_arg,
        start_gazebo_server,
        start_gazebo_client,
        robot_state_publisher_node,
        spawn_robot,
    ])

