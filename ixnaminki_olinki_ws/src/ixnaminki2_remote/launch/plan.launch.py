from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='ixnaminki2_remote',
            executable='plan_server',
            parameters=[ 
                {'use_sim_time': True},
            ],
        ),
    ])