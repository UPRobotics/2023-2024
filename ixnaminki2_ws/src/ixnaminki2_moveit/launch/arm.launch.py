import os
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import Node


def generate_launch_description():
    gazebo = IncludeLaunchDescription(
            os.path.join(
                get_package_share_directory("ixnaminki2_description"),
                "launch",
                "arm_gazebo.launch.py"
            ),
#            launch_arguments={"is_sim": "True"}.items()
        )
    
    controller = IncludeLaunchDescription(
            os.path.join(
                get_package_share_directory("ixnaminki2_control"),
                "launch",
                "arm_controllers.launch.py"
            ),
#            launch_arguments={"is_sim": "True"}.items()
        )
    
    moveit = IncludeLaunchDescription(
            os.path.join(
                get_package_share_directory("ixnaminki2_moveit"),
                "launch",
                "moveit.launch.py"
            ),
#            launch_arguments={"is_sim": "True"}.items()
        )
    
    plan = Node(
            package='ixnaminki2_remote',
            executable='plan_server',
            parameters=[ 
                {'use_sim_time': True},
            ],
        )

    return LaunchDescription([
        moveit,
        controller,
        gazebo,
        plan,
    ])