from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    alexa_interface_node = Node(
        package="ixnaminki2_alexa",
        executable="alexa_interface.py",
        #arguments=[]
    )

    return LaunchDescription([
        alexa_interface_node
    ])
