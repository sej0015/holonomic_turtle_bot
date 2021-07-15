// This node was made to try and turn all three wheels from teleop_twist_joy

//Things to include to make it work
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <dynamixel_workbench_msgs/DynamixelCommand.h>
#include <string>


//Declare some variables
  int maxVel = 255;
  std::string wheel_cmd = "Goal_Velocity";
  int32_t left_vel_out = 0;
  int32_t right_vel_out = 0;
  int32_t back_vel_out = 0;
  double cmd_vel_max = 0.5;
  double cmd_vel_x = 0;
  double cmd_vel_y = 0;
  double cmd_omega = 0;
  std::string joy_topic_name = "cmd_vel";
  std::string dynamixel_service_name = "/dynamixel_workbench/dynamixel_command";
  int loop_rate = 10;

//Declare callback functions
  void joy_topic_Callback(const geometry_msgs::Twist::ConstPtr& msg);

//Start Main
int main(int argc, char** argv)
{

  //Initialize Ros
  std::string node_name = "holonomic_pkg_node";
  ros::init(argc, argv, node_name);
  ros::NodeHandle nh;

  //Declare and read Varaibles that are created in ~/turtle_config/turtle_config.yaml
  int left_wheel_id;
  int right_wheel_id;
  int back_wheel_id;
  int temp_id;


  nh.getParam(node_name+"/right_wheel_id", right_wheel_id );
  //right_wheel_id = temp_id;
  nh.getParam(node_name+"/left_wheel_id",left_wheel_id);
  //left_wheel_id = temp_id;
  nh.getParam(node_name+"/back_wheel_id",back_wheel_id);
  //back_wheel_id = temp_id;

  // Initialize publishers and subscribers
  ros::Subscriber cmd_vel_sub = nh.subscribe(joy_topic_name, 1, joy_topic_Callback);
  ros::ServiceClient dynamixel_client = nh.serviceClient<dynamixel_workbench_msgs::DynamixelCommand>(dynamixel_service_name);
  dynamixel_workbench_msgs::DynamixelCommand dynamixel_service_call;

  ros::Rate rate(loop_rate);

  while(ros::ok())
  {
    dynamixel_service_call.request.command = "";
    dynamixel_service_call.request.addr_name = wheel_cmd;

    dynamixel_service_call.request.id = left_wheel_id;
    dynamixel_service_call.request.value = left_vel_out;
    dynamixel_client.call(dynamixel_service_call);

    dynamixel_service_call.request.id = right_wheel_id;
    dynamixel_service_call.request.value = right_vel_out;
    dynamixel_client.call(dynamixel_service_call);

    dynamixel_service_call.request.id = back_wheel_id;
    dynamixel_service_call.request.value = back_vel_out;
    dynamixel_client.call(dynamixel_service_call);

    //ROS_INFO("command: %s", char(dynamixel_service_call.request.command));
    //ROS_INFO("id: %i", dynamixel_service_call.request.id);
    //ROS_INFO("addr_name: %s", char(wheel_cmd));
    //ROS_INFO("value: %i", dynamixel_service_call.request.value);

    //if (dynamixel_client.call(dynamixel_service_call))
    //{
      //ROS_INFO("Cmd Vel: %i", wheel_vel_out);
    //} else {
    //  ROS_INFO("Bad Cmd");
    //}
    ros::spinOnce();
    rate.sleep();
  }
}


void joy_topic_Callback(const geometry_msgs::Twist::ConstPtr& msg)
{
  cmd_vel_x = msg -> linear.x*-2;
  cmd_vel_y = msg -> linear.y*2;
  cmd_omega = msg -> angular.z*-2;
  double norm = sqrt(cmd_vel_x*cmd_vel_x + cmd_vel_y*cmd_vel_y);
  double unit_x = cmd_vel_x /(norm);
  double unit_y = cmd_vel_y /(norm);
  if((sqrt(cmd_vel_x*cmd_vel_x + cmd_vel_y*cmd_vel_y)) == 0) {
    unit_x = 0;
    unit_y = 0;
  }
  
  right_vel_out = round((0.58*unit_x*norm -0.33*unit_y*norm + 0.33*cmd_omega*2)*maxVel); 
  ROS_INFO("%i", right_vel_out);
  left_vel_out = round((-0.58*unit_x*norm -0.33*unit_y*norm + 0.33*cmd_omega*2)*maxVel);
  back_vel_out = round((0*unit_x*norm + 0.67*unit_y*norm + 0.33*cmd_omega*2)*maxVel);
  //wheel_vel_out = round(cmd_vel/cmd_vel_max * maxVel);
}


















