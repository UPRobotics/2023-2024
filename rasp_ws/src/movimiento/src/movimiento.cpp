#include "rclcpp/rclcpp.hpp"
#include "formatos/srv/movearm.hpp"
#include "formatos/srv/moverob.hpp"
#include <string>
#include <memory>


#include <chrono>
#include <thread>
#include <unistd.h>
#include <algorithm>
#include <stdio.h>

void subscriber_functionR(){
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "moviendo robot");
    return;
}


void addR(const std::shared_ptr<formatos::srv::Moverob::Request> request,
          std::shared_ptr<formatos::srv::Moverob::Response> response)
{
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), 
        "Informacion recibida:\nd1: %d\nd2:: %d\nflippers: %d",
                request->d1, request->d2, request->flippers);
    subscriber_functionR();         
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "No es requerida ninguna respuesta");
  
}

void subscriber_functionA(){

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "moviendo brazo");

    return;
}


void addA(const std::shared_ptr<formatos::srv::Movearm::Request> request,
          std::shared_ptr<formatos::srv::Movearm::Response> response)
{
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Informacion recibida");
    for(int i = 0; i < 6; i++){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), 
            "motor%d: %d",
                    i, request->armi[i]);
    }
    subscriber_functionA();         
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "No es requerida ninguna respuesta");
  
}
int main(int argc, char **argv)
{
	printf("Iniciando servidores para mover brazo y robot\n");
    rclcpp::init(argc, argv);

    std::shared_ptr<rclcpp::Node> move_robot = rclcpp::Node::make_shared("ixnaminki_server_move_robot");

    rclcpp::Service<formatos::srv::Moverob>::SharedPtr serviceR =
        move_robot->create_service<formatos::srv::Moverob>("move_robot", &addR);

  
    std::shared_ptr<rclcpp::Node> move_arm = rclcpp::Node::make_shared("ixnaminki_server_move_arm");

    rclcpp::Service<formatos::srv::Movearm>::SharedPtr serviceA =
        move_arm->create_service<formatos::srv::Movearm>("move_arm", &addA);


    rclcpp::spin(move_robot);
    rclcpp::spin(move_arm);
    rclcpp::shutdown();
}