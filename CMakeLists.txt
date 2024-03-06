cmake_minimum_required(VERSION 3.5)
project(compressed_image_saver)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)

# 导入ROS 2的构建工具
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(OpenCV REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(cv_bridge REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
    "msg/CompressedImage.msg"
)

set(my_node "image_saver")


# 添加可执行文件
add_executable(${my_node} src/${my_node}.cpp)
target_include_directories(${my_node} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(${my_node} ${rclcpp_LIBRARIES})
target_link_libraries(${my_node} ${OpenCV_LIBS})

ament_target_dependencies(${my_node}
    rosidl_default_runtime
    sensor_msgs
    cv_bridge
    OpenCV
)

# 安装可执行文件
install(TARGETS ${my_node}
  DESTINATION lib/${PROJECT_NAME})

# 安装头文件
install(DIRECTORY include/
  DESTINATION include)

# 导出ROS 2软件包
ament_export_dependencies(rclcpp)
ament_package()