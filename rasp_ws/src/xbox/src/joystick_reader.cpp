#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"


#include "std_srvs/srv/set_bool.hpp"
#include "formatos/srv/moverob.hpp"
#include "formatos/srv/movearm.hpp"

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <chrono>
#include <thread>


short modo_de_lectura=0;
bool was_cero=0;

void addCM(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
          std::shared_ptr<std_srvs::srv::SetBool::Response> response)
{
    if(request->data==false){       
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Cambiando modo lectura a movimiento del robot");
        modo_de_lectura=0;
        response->message="movimiento del robot";
    }else{
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Cambiando modo lectura a movimiento del brazo");
        modo_de_lectura=1;
        response->message="movimiento del brazo";
    }
    was_cero=0;
    response->success=true;
    
}


std::shared_ptr<rclcpp::Node> node;

rclcpp::Client<formatos::srv::Moverob>::SharedPtr clientRob;
rclcpp::Client<formatos::srv::Movearm>::SharedPtr clientArm;

void MoveRob(std::vector<double> &axis, std::vector<bool> &botons){
    int r=0, r2=0, flipper=0;

    if((axis[1]<=1.0 && axis[1]>0.2) || (axis[1]>=-1.0 && axis[1]<-0.2)){ 
        r = 1;  
        r2 = axis[1];
    }// hacia adelante y atras
    else if((axis[0]<=1.0 && axis[0]>0.2) || (axis[0]>=-1.0 && axis[0]<-0.2) ){ 
        r = 2;  
        r2 = axis[0];
    }// hacia izquierda
    if(r==0){
        if(was_cero) return;
	    was_cero=true;
    }else{
	    was_cero=false;
    }

    auto request = std::make_shared<formatos::srv::Moverob::Request>();

    request->d1 = r;
    request->d2 = r2;
    request->flippers = flipper;


    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending: r1=[%d]        r2=[%d]       flipper=[%d]", r, r2, flipper);
    auto result = clientRob->async_send_request(request);
    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "succes: %d", true);
    }else{
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service direcciones");
    }
    return;
}

void MoveArm(std::vector<double> &axis, std::vector<bool> &botons){

    std::vector<float> armi(6, 0);
    //establecer que botones hacen que  cosass
    
    auto request = std::make_shared<formatos::srv::Movearm::Request>();

    bool z = true;
    for(int i = 0; i < 6; i++){
        if(armi[i]!=0)  z=false;
    }
    if(z){
        if(was_cero) return;
        was_cero=true;
    }else{
        was_cero=false;
    }


    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending: ");
    for(int i = 0; i < 6; i++){
        request->armi[i]=armi[i];
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "direccion del grado[%d]: %f", i, armi[i]);
    }
    auto result = clientArm->async_send_request(request);
    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "succes: %d", true);
    }else{
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service direcciones");
    }
    return;
}


const double death_zone=10;
void joyCallback(const sensor_msgs::msg::Joy::SharedPtr joy){

    std::vector<double> axis(8);

    std::vector<bool> botons(12);

    for(int i = 0; i < 8; i++) axis[i] = joy->axes[i];

    for(int i = 0; i < 12; i++) botons[i]=joy->buttons[i]; 

    /*

        axis 0 joystick de arriba horizontal

        axis 1 joystick de arriba vertical

        axis 2 LT

        axis 3 joystick de abajo horizontal

        axis 4 joystick de abajo vertical

        axis 5 RT

        axis 6 flechita horizontal

        axis 7 flechita vertical

        boton 0 es A

        boton 1 es B

        boton 2 es X

        boton 3 es Y

        boton 4 es LB

        boton 5 es RB

        boton 6 es doble cuadrito

        boton 7 es 3 lineas

        boton 8 es xbox

        boton 9 es presionar ruedita de arriba

        boton 10 es presionar ruedita de abajo

        boton 11 es captura

    */





/*
    if(axis[1]<=1.0 && axis[1]>0.2) r = std::min(0 + (int)(axis[1] * 100), 99);

    else if(axis[1]>=-1.0 && axis[1]<-0.25) r = std::min(100 + (int)(axis[1] * -100), 199);

    else if(axis[0]<=1.0 && axis[0]>0.25) r = std::min(200 + (int)(axis[0] * 100), 299);

    else if(axis[0]>=-1.0 && axis[0]<-0.25) r = std::min(300 + (int)(axis[0] * -100), 399);

    else if(axis[3]>=-1.0 && axis[3]<=-0.25) r = std::min(400 + (int)(axis[3] * -100), 499);

    else if(axis[3]<=1.0 && axis[3]>=0.25) r = std::min(500 + (int)(axis[3] * 100), 599);

    else if(axis[4]>=-1.0 && axis[4]<=-0.25) r = std::min(600 + (int)(axis[4] * -100), 699);

    else if(axis[4]<=1.0 && axis[4]>=0.25) r = std::min(700 + (int)(axis[4] * 100), 799);
    
   
    else if(axis[2]==-1.0) r = 14;

    else if(axis[6]==-1.0) r = 10;

    else if(axis[6]==1.0) r = 9;

    else if(axis[7]==-1.0) r = 8;

    else if(axis[7]==1.0) r = 7;

    else if(botons[0]) r = 16;

    else if(botons[1]) r = 0;

    else if(botons[2]) r = 13;

    else if(botons[3]) r = 15;

    else if(botons[4]) r = 12;

    else if(botons[5]) r = 11;

    else if(botons[6]) r = 0;

    else if(botons[7]) r = 0;

    else if(botons[8]) r = -1;

    else if(botons[9]) r = 0;

    else if(botons[10]) r = 0;

    else if(botons[11]) r = 0;
*/

    if(modo_de_lectura==0) MoveRob(axis, botons);
    else MoveArm(axis, botons);

    std::this_thread::sleep_for(std::chrono::milliseconds((200)));

    return;
}



int main(int argc, char ** argv){

    rclcpp::init(argc, argv);
    
    std::shared_ptr<rclcpp::Node> cambiar_modo = rclcpp::Node::make_shared("cambiar_modo_de_lectura");

    rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr serviceCM =
        cambiar_modo->create_service<std_srvs::srv::SetBool>("cambiar_modo", &addCM);


    auto nodes = rclcpp::Node::make_shared("joystick_reader");

    auto joy_sub = nodes->create_subscription<sensor_msgs::msg::Joy>("/joy",10,joyCallback);

	node = rclcpp::Node::make_shared("reader_client");

	clientRob = node->create_client<formatos::srv::Moverob>("direcciones");	
	clientArm = node->create_client<formatos::srv::Movearm>("direcciones");	

    rclcpp::spin(nodes);

    rclcpp::shutdown();

    return 0;

}
