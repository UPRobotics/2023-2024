#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

#include <memory>
#include <string>
#include <vector>


#include "std_srvs/srv/set_bool.hpp"

#include <memory>


using namespace std::placeholders;

class TaskServer : public rclcpp::Node
{
public:
    TaskServer() : Node("task_server")
    {
        service_ = create_service<std_srvs::srv::SetBool>("task", 
        std::bind(&TaskServer::serviceCallback, this, _1, _2));
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Service task Ready");
    }


private:
    rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr service_;

    void serviceCallback(const std::shared_ptr<std_srvs::srv::SetBool::Request> req,
                         const std::shared_ptr<std_srvs::srv::SetBool::Response> res)
    {
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "New Request Received");
        auto arm_move_group = moveit::planning_interface::MoveGroupInterface(shared_from_this(), "arm");
        RCLCPP_INFO(get_logger(), "Executing goals :D");

        double rad = arm_move_group.getGoalPositionTolerance();
        RCLCPP_INFO(get_logger(), "EL radio es: %f", rad);


        double x = 0.0015 * 200, y = 10, z = 10; 
        std::string link = "gripper_base";

        bool arm_within_bounds = arm_move_group.setPositionTarget(x,x,x, link);
        
       /*
       std::vector<double> orale(5, -0.3);
        bool arm_within_bounds = arm_move_group.setJointValueTarget(orale);*/
        if (!arm_within_bounds)
        {
          RCLCPP_WARN(get_logger(), "Target joint position(s) were outside of limits, but we will plan and clamp to the limits ");
          return;
        }

        moveit::planning_interface::MoveGroupInterface::Plan arm_plan;
        bool arm_plan_success = (arm_move_group.plan(arm_plan) == moveit::core::MoveItErrorCode::SUCCESS);
        
        if(arm_plan_success)
        {
          RCLCPP_INFO(get_logger(), "Planner SUCCEED, moving the arm");
          arm_move_group.move();
        }
        else
        {
          RCLCPP_ERROR(get_logger(), "plan failed!");
          return;
        }
      
        RCLCPP_INFO(get_logger(), "Goal succeeded");
    }

};


int main(int argc, char* argv[])
{
  std::cout<<"a";
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TaskServer>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}

/*#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

#include <memory>
#include <string>
#include <vector>


using namespace std::placeholders;

namespace ixnaminki2_instrucciones
{
class TaskServer : public rclcpp::Node
{
public:
    explicit TaskServer(const rclcpp::NodeOptions& options = rclcpp::NodeOptions())
    : Node("task_server", options)
    {
        RCLCPP_INFO(get_logger(), "Starting the Server");
        RCLCPP_INFO(get_logger(), "Executing goals");
    //    execute();
    }

//private:
  void execute()
  {

    // MoveIt 2 Interface
    auto arm_move_group = moveit::planning_interface::MoveGroupInterface(shared_from_this(), "arm");
    RCLCPP_INFO(get_logger(), "Executing goals");

    std::string a = arm_move_group.getEndEffectorLink();
    RCLCPP_INFO(get_logger(), "this is: %s", a);
    return;

    double x = 1, y = 1, z = 1; 
    std::string link = "gripper_base";

    bool arm_within_bounds = arm_move_group.setPositionTarget(x,y,z, link);
    
    if (!arm_within_bounds)
    {
      RCLCPP_WARN(get_logger(),
                  "Target joint position(s) were outside of limits, but we will plan and clamp to the limits ");
      return;
    }

    moveit::planning_interface::MoveGroupInterface::Plan arm_plan;
    bool arm_plan_success = (arm_move_group.plan(arm_plan) == moveit::core::MoveItErrorCode::SUCCESS);
    
    if(arm_plan_success)
    {
      RCLCPP_INFO(get_logger(), "Planner SUCCEED, moving the arm");
      arm_move_group.move();
    }
    else
    {
      RCLCPP_ERROR(get_logger(), "plan failed!");
      return;
    }
  
    RCLCPP_INFO(get_logger(), "Goal succeeded");
  }
};
}  

RCLCPP_COMPONENTS_REGISTER_NODE(ixnaminki2_instrucciones::TaskServer)
*/