#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include <opencv4/opencv2/opencv.hpp>
#include <chrono>
#include <ctime>

class Image_Saver : public rclcpp::Node {
    public:
        using CompressedImage = sensor_msgs::msg::CompressedImage;
        int cnt = 0;
        std::string _path;
    private:
        void _callback(CompressedImage::SharedPtr compressed_image_ptr) {
            // printf("[image_saver] callback in\n");
            cv_bridge::CvImagePtr cv_image_ptr = cv_bridge::toCvCopy(compressed_image_ptr, "bgr8");
            // printf("[image_saver] image size = %d * %d\n", cv_image_ptr->image.cols, cv_image_ptr->image.rows);
            char* p_file_name = (char *) malloc(255 * sizeof(char));
            sprintf(p_file_name, "%s/camera_%04d.jpg", _path.data(), cnt);
            cnt++;
            cv::imwrite(p_file_name, cv_image_ptr->image);
            // printf("[image_saver] image '%s' stored\n", p_file_name);
            free(p_file_name);
        }
        rclcpp::Subscription<CompressedImage>::SharedPtr _subscription;
    public:
        Image_Saver(std::string path, std::string topic, int hz) : Node("image_saver") {
            if(path[path.length() - 1] == '/')
                _path = path.substr(0, path.length() - 1);
            else _path = path;
            _subscription = this->create_subscription<CompressedImage>(
                topic,
                hz,
                std::bind(&Image_Saver::_callback, this, std::placeholders::_1)
            );
        }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    if(argc != 4) {
        printf("usage: ros2 run compressed_image_saver image_saver <store-path> <topic-name> <frequency>\n");
        rclcpp::shutdown();
        return 1;
    }
    auto image_saver = std::make_shared<Image_Saver>(argv[1], argv[2], atoi(argv[3]));
    rclcpp::spin(image_saver);
    rclcpp::shutdown();
    return 0;
}