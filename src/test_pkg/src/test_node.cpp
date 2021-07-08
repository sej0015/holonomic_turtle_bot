// This node was made to try and turn one wheel using cmd_vel from teleop_twist_joy

//Things to include to make it work
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <dynamixel_workbench_msgs/DynamixelCommand.h>
#include <string>


//Declare some variables
  int maxVel = 255;
  uint8_t wheel_id = 1;
  std::string wheel_cmd = "Goal_Velocity";
  int32_t wheel_vel_out = 0;
  double cmd_vel_max = 0.5;
  double cmd_vel = 0;
  std::string joy_topic_name = "cmd_vel";
  std::string dynamixel_service_name = "/dynamixel_workbench/dynamixel_command";
  int loop_rate = 10;

//Declare callback functions
  void joy_topic_Callback(const geometry_msgs::Twist::ConstPtr& msg);

//Start Main
int main(int argc, char** argv)
{

  //Initialize Ros
  std::string node_name = "test_node";
  ros::init(argc, argv, node_name);
  ros::NodeHandle nh;

  // Initialize publishers and subscribers
  ros::Subscriber cmd_vel_sub = nh.subscribe(joy_topic_name, 1, joy_topic_Callback);
  ros::ServiceClient dynamixel_client = nh.serviceClient<dynamixel_workbench_msgs::DynamixelCommand>(dynamixel_service_name);
  dynamixel_workbench_msgs::DynamixelCommand dynamixel_service_call;

  ros::Rate rate(loop_rate);

  while(ros::ok())
  {
    dynamixel_service_call.request.command = "";
    dynamixel_service_call.request.id = wheel_id;
    dynamixel_service_call.request.addr_name = wheel_cmd;
    dynamixel_service_call.request.value = wheel_vel_out;
    //ROS_INFO("command: %s", char(dynamixel_service_call.request.command));
    ROS_INFO("id: %i", dynamixel_service_call.request.id);
    //ROS_INFO("addr_name: %s", char(wheel_cmd));
    ROS_INFO("value: %i", dynamixel_service_call.request.value);
    if (dynamixel_client.call(dynamixel_service_call))
    {
      ROS_INFO("Cmd Vel: %i", wheel_vel_out);
    } else {
      ROS_INFO("Bad Cmd");
    }
    ros::spinOnce();
    rate.sleep();
  }
}


void joy_topic_Callback(const geometry_msgs::Twist::ConstPtr& msg)
{
  cmd_vel = msg -> linear.x;
  wheel_vel_out = round(cmd_vel/cmd_vel_max * maxVel);
}









