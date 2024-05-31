#include "rclcpp/rclcpp.hpp"
#include "formato/srv/moverob.hpp"

#include <chrono>
#include <memory>


using namespace std::chrono_literals;
using std::placeholders::_1;


double x=-1000,y,z;
const double inc=25;

class PlanClient : public rclcpp::Node 
{
public:
    PlanClient(double X, double Y, double Z) : Node("ixnaminki_server_move_robot")
    {
        client_ = create_client<formato::srv::Moverob>("move_robot");

        auto request = std::make_shared<formato::srv::Moverob::Request>();
        request->d1 = x;
        request->d2 = y;
        request->flippers = z;/*
        while (!client_->wait_for_service(1s)) {
            if (!rclcpp::ok()) 
            {
                RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
                return;
            }
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
        }*/
        auto result = client_->async_send_request(request, std::bind(&PlanClient::responseCallback, this, _1));
    }

private:
    rclcpp::Client<formato::srv::Moverob>::SharedPtr client_;

    void responseCallback(rclcpp::Client<formato::srv::Moverob>::SharedFuture future)
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