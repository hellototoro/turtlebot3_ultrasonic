#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "Ultrasonic.hpp"

using namespace std::chrono_literals;

static constexpr int trigPin = 3;
static constexpr int echoPin = 4;


class MinimalPublisher : public rclcpp::Node
{
    public:
    MinimalPublisher() : Node("minimal_publisher"), sensor(trigPin, echoPin) {
        publisher_ = this->create_publisher<std_msgs::msg::Float64>("/sensor/distance", 10);
        timer_ = this->create_wall_timer(
            500ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

    void run(double speed) {
        auto message = std_msgs::msg::Float64();
        message.data = speed;
        RCLCPP_INFO(this->get_logger(), "Publishing: %f", message.data);
        publisher_->publish(message);
    }

private:
    void timer_callback() {
        auto message = std_msgs::msg::Float64();
        message.data = sensor.getDistance();
        RCLCPP_INFO(this->get_logger(), "Publishing: %f", message.data);
        publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
    Ultrasonic sensor;
};

void signal_handler(int signum) {
    exit(signum);
}

int main(int argc, char * argv[])
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalPublisher>());
    rclcpp::shutdown();
    return 0;
}
