#include <rclcpp/rclcpp.hpp>
#include "posicion/srv/target.hpp"

#include <chrono>
#include <memory>


using namespace std::chrono_literals;
using std::placeholders::_1;


class PlanClient : public rclcpp::Node 
{
public:
    PlanClient(double X, double Y, double Z) : Node("plan_client")
    {
        client_ = create_client<posicion::srv::Target>("plan");

        auto request = std::make_shared<posicion::srv::Target::Request>();
        request->x = X;
        request->y = Y;
        request->z = Z;

        while (!client_->wait_for_service(1s)) {
            if (!rclcpp::ok()) 
            {
                RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
                return;
            }
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
        }

        auto result = client_->async_send_request(request, std::bind(&PlanClient::responseCallback, this, _1));
    }

private:
    rclcpp::Client<posicion::srv::Target>::SharedPtr client_;

    void responseCallback(rclcpp::Client<posicion::srv::Target>::SharedFuture future)
    {
        if(future.valid())
        {
            RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Service Response succes");
        }
        else
        {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Service Failure");
        }
        rclcpp::shutdown();
    }
};


int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);

  if (argc != 4) {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "X Y Z!");
    return 1;
  }

  auto node = std::make_shared<PlanClient>(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}