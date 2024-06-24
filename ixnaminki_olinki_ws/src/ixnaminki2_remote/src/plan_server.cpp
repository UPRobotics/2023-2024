#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <time.h>

#include "posicion/srv/target.hpp"

#include<memory>
#include<string>
#include<vector>

const double esc = 0.0015;

using namespace std::placeholders;



class PlanServer : public rclcpp::Node {
public: 
    PlanServer() : Node("plan_server"){
        service_ = create_service<posicion::srv::Target>("plan", 
        std::bind(&PlanServer::execute, this, _1, _2));
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "SERVIDOR INICIADO");
    }
private:
    rclcpp::Service<posicion::srv::Target>::SharedPtr service_;
    void execute(const std::shared_ptr<posicion::srv::Target::Request> req,
                 const std::shared_ptr<posicion::srv::Target::Response> res){
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Instruccion Recibida");

        auto arm = moveit::planning_interface::MoveGroupInterface(shared_from_this(), "arm");

        //control_msgs/msg/DynamicJointState

        double x = esc * req->x, y = esc * req->y, z = esc * req->z;

        bool se_puede = arm.setPositionTarget(x, y, z, "gripper_base");

        if(!se_puede){
            RCLCPP_ERROR(get_logger(), "No se puede alcanzar el objetivo");
            res->exito = false;
            return;
        }

        moveit::planning_interface::MoveGroupInterface::Plan planecito;
        bool se_pudo = (arm.plan(planecito) == moveit::core::MoveItErrorCode::SUCCESS);



        if(se_pudo){
            arm.move();
        }else{
            RCLCPP_ERROR(get_logger(), "Error");
            res->exito = false;
            return;
        }
        RCLCPP_INFO(get_logger(), "Si se pudo banda, asi se ve");
        res->exito = true;



        return;
        bool r = arm.startStateMonitor(0.0);
        RCLCPP_INFO(get_logger(), "se pudo? %d",r);



        
        RCLCPP_INFO(get_logger(), "PRUEBA");
        double wait = 1.0;
        auto csr = arm.getCurrentState(2.0);
        RCLCPP_INFO(get_logger(), "se pudo");


        std::vector<double> angulos = arm.getCurrentJointValues();
        int n = angulos.size();
        RCLCPP_INFO(get_logger(), "datos? %d",n);
      /*  for(int i = 0; i < 5; i++){
            RCLCPP_INFO(get_logger(), "Angulo del motor %d: %f", i, angulos[i]);
            res->motores[i] = angulos[i];
        }*/
        auto b = arm.getCurrentPose("gripper_base");
            RCLCPP_INFO(get_logger(), "mhm");

        return;
        

    }
};

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PlanServer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}