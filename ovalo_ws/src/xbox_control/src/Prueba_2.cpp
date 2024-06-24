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
int motor = 0;

void addCM(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
          std::shared_ptr<std_srvs::srv::SetBool::Response> response)
{
	modo_de_lectura = request->data;
	RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Cambiando modo lectura a movimiento del robot");
	response->message="movimiento del robot";
    response->success=true;
    
}


std::shared_ptr<rclcpp::Node> node;

rclcpp::Client<formatos::srv::Moverob>::SharedPtr clientRob;
rclcpp::Client<formatos::srv::Movearm>::SharedPtr clientArm;


void MoveRob(std::vector<double> &axis, std::vector<bool> &botons, short modo){
    double r=0, r2=0, flipper=0;

    double variable = 2;
    if(modo == 1 && !switch1) vector<double> velocidades = [axis[1], axis[4]]; //si es el modo 1 (rapido) y el switch esta para mover el robot entonces se movera con respecto a los dos joysticks.
    if(modo == 0 && !switch1) vector<double> velocidades = [axis[1]/variable, axis[4]/variable]; //si es el modo 0 (lento) y el switch esta para mover el robot entonces se movera con respecto a los dos joysticks.



    if(modo == 5){
        if(boton[2]) flipper = 2; // Flippers de enfrente hacia abajo
        if(boton[3]) flipper = 1; // Flippers de enfrente hacia arriba
        if(boton[0]) flipper = 3; // Flippers de atras hacia arriba
        if(boton[1]) flipper = 4; // Flippers de atras hacia abajo
    }

    auto request = std::make_shared<formatos::srv::Moverob::Request>();

    request->d1 = r;
    request->d2 = r2;
    request->flippers = flipper;


    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending: r1=[%d]        r2=[%f]       flipper=[%f]", r, r2, flipper);
    auto result = clientRob->async_send_request(request);
    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "succes: %d", true);
    }else{
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service direcciones");
    }
    return;
}


const int girar_horario = 3000;
const int girar_antihorario = 3001;
const int sin_giro = 3002;

void MoveArm(std::vector<double> &axis, std::vector<bool> &botons){

    std::vector<float> armi(6, sin_giro);
    //establecer que botones hacen que  cosass
    if(botons[4]) motor--;
    if(botons[5]) motor++;
    
    auto request = std::make_shared<formatos::srv::Movearm::Request>();
    
    bool z = true;
    for(int i = 0; i < 6; i++){
        if(armi[i]!=0)  z=false;
    }

	if(axis[1]>= 0.2){
		armi[motor] = girar_horario; // valor predeterminado
	}else if(axis[1]<=-0.2){
		armi[motor] = girar_antihorario; // valor predeterminado
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


    if(modo_de_lectura==0 || modo_de_lectura == 1) MoveRob(axis, botons, modo_de_lectura);
    else MoveArm(axis, botons);

    std::this_thread::sleep_for(std::chrono::milliseconds((100)));//?

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
