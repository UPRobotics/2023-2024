#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/srv/set_parameters.hpp"
#include "formatosa/srv/movearm.hpp"
#include "formatosa/srv/moverob.hpp"
#include "movimiento/myserial.hpp"
#include <string>
#include <memory>


#include <chrono>
#include <thread>
#include <unistd.h>
#include <algorithm>
#include <stdio.h>

int COMM_SET_DUTY = 5;
std::string motor_indexes[4]={"/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyACM2", "/dev/ttyACM3"};

const unsigned short crc16_tab[] = { 0x0000, 0x1021, 0x2042, 0x3063, 0x4084,
		0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad,
		0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7,
		0x62d6, 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a,
		0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672,
		0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719,
		0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7,
		0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948,
		0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50,
		0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b,
		0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97,
		0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe,
		0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca,
		0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3,
		0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d,
		0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214,
		0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c,
		0xc50d, 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3,
		0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d,
		0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806,
		0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e,
		0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1,
		0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b,
		0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0,
		0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 };

unsigned short crc16(unsigned char *buf, unsigned int len) {
	unsigned int i;
	unsigned short cksum = 0;
	for (i = 0; i < len; i++) {
		cksum = crc16_tab[(((cksum >> 8) ^ *buf++) & 0xFF)] ^ (cksum << 8);
	}
	return cksum;
}

int PackSendPayload(uint8_t* payload, int lenPay, int num, int motor_index) {
	uint16_t crcPayload = crc16(payload, lenPay);
	std::cout<<crcPayload<<" : ";
	int count = 0;
	uint8_t messageSend[256];

	if (lenPay <= 256)
	{
		messageSend[count++] = 2;
		messageSend[count++] = lenPay;
	}
	else
	{
		messageSend[count++] = 3;
		messageSend[count++] = (uint8_t)(lenPay >> 8);
		messageSend[count++] = (uint8_t)(lenPay & 0xFF);
	}
	memcpy(&messageSend[count], payload, lenPay);

	count += lenPay;
	messageSend[count++] = (uint8_t)(crcPayload >> 8);
	messageSend[count++] = (uint8_t)(crcPayload & 0xFF);
	messageSend[count++] = 3;
	messageSend[count] = NULL;


#ifdef DEBUG
	DEBUGSERIAL.print("UART package send: "); SerialPrint(messageSend, count);

#endif // DEBUG*/

    mySerial serial(motor_indexes[motor_index],115200);
	bool respuesta = serial.Send(messageSend, count);
	serial.Close();
	return count;
}


void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
	buffer[(*index)++] = number >> 24;
	buffer[(*index)++] = number >> 16;
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;
}

void VescUartSetDuty(float duty, int num, int motor_index) {
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_DUTY;
	//std::cout<<payload<<"---";
	buffer_append_int32(payload, (int32_t)(duty * 100000), &index);
	PackSendPayload(payload, 5, num, motor_index);
}
void VescUartSetDuty(float duty, int motor_index) {
	VescUartSetDuty(duty, 0, motor_index);
}



double current_motor_velocity[4]={0.0,0.0,0.0,0.0};
const double incremento = 0.1;

void subscriber_functionR(double r1, double r2, double flippers){
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "moviendo robot");
    
	if(flippers!=0){
		if(flippers==1){
			VescUartSetDuty(0.2, 2);
		}else if(flippers==2){
			VescUartSetDuty(-0.2, 2);
		}else if(flippers==3){
			VescUartSetDuty(0.2, 3);
		}else{
			VescUartSetDuty(-0.2, 3);
		}
		return;
	}

    double r[2] = {r2, r2};
    if(r1==2) r[1] *= -1.0;

    for(int k = 0; k < 2; k++){
        if(current_motor_velocity[k]<r[k]){
            current_motor_velocity[k] = std::min(current_motor_velocity[k] + incremento, r[k]);
        }else if(current_motor_velocity[k]>r[k]){
            current_motor_velocity[k] = std::max(current_motor_velocity[k] - incremento, r[k]);
        }
        VescUartSetDuty(current_motor_velocity[k], k);
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), 
        "Velocidad:\nmotor 1: %f\nmotor 2: %f",
                current_motor_velocity[0], current_motor_velocity[1]);
    return;
}


void addR(const std::shared_ptr<rcl_interfaces::srv::SetParameters::Request> request,
	std::shared_ptr<rcl_interfaces::srv::SetParameters::Response> response)
{
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), 
        "Informacion recibida:\nd1: %d\nd2:: %d\nflippers: %d",
                request->parameters[0].value.double_array_value[0], request->parameters[0].value.double_array_value[1], request->parameters[0].value.double_array_value[2]);
    subscriber_functionR(request->parameters[0].value.double_array_value[0], request->parameters[0].value.double_array_value[1], request->parameters[0].value.double_array_value[2]);        
  
}

void subscriber_functionA(){

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "moviendo brazo");

    return;
}


void addA(const std::shared_ptr<formatosa::srv::Movearm::Request> request,
          std::shared_ptr<formatosa::srv::Movearm::Response> response)
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

    std::shared_ptr<rclcpp::Node> move = rclcpp::Node::make_shared("ixnaminki_server_move_robot");

    rclcpp::Service<rcl_interfaces::srv::SetParameters>::SharedPtr serviceR = move->create_service<rcl_interfaces::srv::SetParameters>("move_robot", &addR);

    /*rclcpp::Service<formatosa::srv::Movearm>::SharedPtr serviceA =
        move->create_service<formatosa::srv::Movearm>("move_arm", &addA);*/


    rclcpp::spin(move);
    rclcpp::shutdown();
}
