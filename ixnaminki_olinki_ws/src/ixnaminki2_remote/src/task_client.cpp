#include <rclcpp/rclcpp.hpp>
#include "std_srvs/srv/set_bool.hpp"

#include <chrono>
#include <memory>


using namespace std::chrono_literals;
using std::placeholders::_1;


class TaskClient : public rclcpp::Node 
{
public:
    TaskClient(int a) : Node("task_client")
    {
        client_ = create_client<std_srvs::srv::SetBool>("task");

        auto request = std::make_shared<std_srvs::srv::SetBool::Request>();

        while (!client_->wait_for_service(1s)) {
            if (!rclcpp::ok()) 
            {
                RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
                return;
            }
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
        }

        auto result = client_->async_send_request(request, std::bind(&TaskClient::responseCallback, this, _1));
    }

private:
    rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr client_;

    void responseCallback(rclcpp::Client<std_srvs::srv::SetBool>::SharedFuture future)
    {
        if(future.valid())
        {
            RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Service Response succes");
        }
        else
        {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Service Failure");
        }
    }
};


int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);

  if (argc != 2) {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Wrong number of arguments! A ");
    return 1;
  }

  auto node = std::make_shared<TaskClient>(atoi(argv[1]));
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}