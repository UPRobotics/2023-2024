#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "control_msgs/msg/dynamic_joint_state.hpp"
#include "formatos/srv/movearm.hpp"

using std::placeholders::_1;

char state = 2;
std::vector<double> last_joints;
const double presicion = 0.0001;

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber()
    : Node("minimal_subscriber")
    {
      subscription_ = this->create_subscription<control_msgs::msg::DynamicJointState>(
      "dynamic_joint_states", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
      last_joints.resize(6, 0);
      clientCambioEjes = node->create_client<formatos::srv::Movearm>("cambio_ejes");
    }

  private:
    void topic_callback(const control_msgs::msg::DynamicJointState::SharedPtr msg) const
    {
        std::vector<double> joints(6);
        int i = 0;
        double max_change = 0;
        for(double &x: joints){
            x = msg->interface_values[i].values[0];
            max_change = std::max(max_change, abs(x - last_joints[i]));
            i++;
        }
        if(state==2){
            state=0;
            last_joints = joints;
            return;
        }
        // el estado 0 es ningun cambio
        // 4 decimales para mantenerse en estado 0
        if(state==0 && max_change>presicion){
            state=1;
            RCLCPP_INFO(this->get_logger(), "Posicion inicial: [%.2f] [%.2f] [%.2f] [%.2f] [%.2f] [%.2f]", 
                msg->interface_values[0].values[0],msg->interface_values[1].values[0], msg->interface_values[2].values[0], msg->interface_values[3].values[0],
                msg->interface_values[4].values[0], msg->interface_values[5].values[0]);
        }else if(state==1 && max_change<presicion){
            state=0;
            RCLCPP_INFO(this->get_logger(), "Posicion final: [%.2f] [%.2f] [%.2f] [%.2f] [%.2f] [%.2f]", 
                msg->interface_values[0].values[0],msg->interface_values[1].values[0], msg->interface_values[2].values[0], msg->interface_values[3].values[0],
                msg->interface_values[4].values[0], msg->interface_values[5].values[0]);

                auto request = std::make_shared<formatos::srv::Movearm::Request>();

                for(int i = 0; i < 6; i++){
                    request->armi[i]=msg->interface_values[i].values[0];
                }
                auto result = clientCambioEjes->async_send_request(request);
                if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS){
                    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "succes: %d", true);
                }else{
                    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service direcciones");
                }

            
        }
        last_joints = joints;

      //  RCLCPP_INFO(this->get_logger(), "[%s] [%.2Lf] [%.2f] [%.2f] [%.2f] [%.2f] [%.2f]", msg->joint_names[0].c_str(),
      //  msg->interface_values[0].values,msg->interface_values[1].values, msg->interface_values[2].values, msg->interface_values[3].values,
      //  msg->interface_values[4].values, msg->interface_values[5].values);
    }
    rclcpp::Client<formatos::srv::Movearm>::SharedPtr clientCambioEjes;
    rclcpp::Subscription<control_msgs::msg::DynamicJointState>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
