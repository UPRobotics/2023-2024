#include "rclcpp/rclcpp.hpp"
#include "structuras/srv/movement.hpp"
#include <string>
#include <memory>


#include <chrono>
#include <thread>
#include <unistd.h>
#include <pigpiod_if2.h>
#include <algorithm>
#include <stdio.h>
#include <math.h>

int pi;
int handle; 

void set_speed(int vel, int pin){
    int x1 = (vel / 254) + 1, x2 = (vel % 254) + 1;
    serial_write_byte(pi, handle, (uint8_t)(00));
    serial_write_byte(pi, handle, (uint8_t)(pin));
    serial_write_byte(pi, handle, (uint8_t)(01));
    serial_write_byte(pi, handle, (uint8_t)(x1));
    serial_write_byte(pi, handle, (uint8_t)(x2));
    serial_write_byte(pi, handle, (uint8_t)(255));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return;
}

const float pib4=0.7853981634;
void move(int d1, int d2){
    float r1 = (d1%100)/100.0;
    float r2 = (d2%100)/100.0;
    float angle = atan(r1/std::max(r2, float(0.0001)));
    float vel_magnitude = sqrt(r1*r1 + r2*r2);
    bool a=0,b=(angle<pib4);
    int vel1 = vel_magnitude * 1000.0; vel1 = std::min(vel1, 999);
    int vel2 = float(vel1) * abs((pib4 - angle)/pib4); vel2 = std::min(vel2, 999);
    if(d2>100){
        std::swap(vel1,vel2);
        a = !a;
        b = !b;
    }
    if(d1>100){
        a = !a;
        b = !b;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "velocidades: %d %d %d %d d2: [%d]\nDebugeo: %f %f %f %f",
        vel1, vel2, a, b, d2, r1, r2, angle, vel_magnitude);
    int j = 200, j2 = 200;
    while(j<=vel1 && j2<=vel2){
        set_speed( a*1000 + j, 1);
        set_speed( b*1000 + j2, 2);
        j+=50;
        j2+=50;
    }
    while(j<=vel1){
        set_speed( a*1000 + j, 1);
        j+=50;
    }
    while(j2<=vel2){
        set_speed( b*1000 + j2, 2);
        j2+=50;
    }
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return;
}

void flipper(int dir){
    set_speed(((dir/100)%2)*1000 + (dir%100)*10, dir/200);
}


const int actuator_pins[2][3]={13,19,26,16,20,21};
const int standar_speed_actuators=130;
int actuator_dir[4][2]={
    standar_speed_actuators, 0, 
    0, standar_speed_actuators,
    standar_speed_actuators, 0,
    0, standar_speed_actuators
};
void arm(int dir){
    set_PWM_dutycycle(pi, actuator_pins[dir/2][0], actuator_dir[dir][0]);
    set_PWM_dutycycle(pi, actuator_pins[dir/2][1], actuator_dir[dir][1]);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
    return;
}

void stop_all(){
    for(int i = 0; i < 2; i++){
        set_speed(0, i+1);
        set_PWM_dutycycle(pi, actuator_pins[i][0], 0);
        set_PWM_dutycycle(pi, actuator_pins[i][1], 0);
    }
    return;
}

void subscriber_function(int instruction, int instruction2){


    if(instruction==-1) {
        printf("gpioTerminate()...\n");
        pigpio_stop(pi);
        return;
    }
    if(instruction==0 && instruction2==0) stop_all();
    else if(instruction>200) arm(instruction-201);
    else if(instruction<=200 || instruction2<=200) move(instruction, instruction2);
    //flipper(instruction-400);


    return;
}


void add(const std::shared_ptr<structuras::srv::Movement::Request> request,
          std::shared_ptr<structuras::srv::Movement::Response>      response)
{
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request\ndirection: %ld %ld",
        request->direction, request->direction2);
    subscriber_function(request->direction, request->direction2); 
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending back response");
}

int main(int argc, char **argv)
{
	printf("Running pigpio program\n");
	pi = pigpio_start(nullptr, nullptr);
    if(pi<0){
        printf("failed");
        return 1;
    }

    for(int i = 0; i < 2; i++) for(int j=0; j < 3; j++) set_mode(pi, actuator_pins[i][j], PI_OUTPUT);
    gpio_write(pi, actuator_pins[0][2], 1);

    handle = serial_open(pi, "/dev/ttyS0", 115200, 0);

    rclcpp::init(argc, argv);

    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("martino_server");
        
    rclcpp::Service<structuras::srv::Movement>::SharedPtr service =
        node->create_service<structuras::srv::Movement>("direcciones", &add);

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to move the robots");
    
    
    rclcpp::spin(node);
    rclcpp::shutdown();
}
