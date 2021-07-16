// Proportional controller


// This node was made to try and turn all three wheels from teleop_twist_joy

// Things to include to make it work
// TODO: Include a different type of message
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <dynamixel_workbench_msgs/DynamixelCommand.h>
#include <string>
#include <geometry_msgs/TransformStamped.h>

//Declare some variables

  std::string waypoint_topic_name = "waypoint_topic"; // TODO: Change this later to waypoint topic name
  std::string vicon_topic_name = "vicon/turtle3/turtle3"; //TODO: Change this later
  std::string cmd_vel_topic_name = "cmd_vel";
  int loop_rate = 10;
  double waypoint_x = 0; // waypoints that this node recieves. The goal locations. 
  double waypoint_y = 0;
  double true_x = 0; // current pose according to the vicon sensors
  double true_y = 0;
	double max_velocity = 0.5; // max velocity of 0.5 units/s
  
//Declare callback functions.
  void waypoint_topic_Callback(const geometry_msgs::Twist::ConstPtr& msg); //TODO: Change type
  void vicon_topic_Callback(const geometry_msgs::TransformStamped::ConstPtr& msg); //TODO: Change Type
  

//Start Main
int main(int argc, char** argv)
{

  //Initialize Ros
  std::string node_name = "proportional_control_node";
  ros::init(argc, argv, node_name);
  ros::NodeHandle nh;

  // Initialize publishers and subscribers
  
ros::Subscriber waypoint_subscriber = nh.subscribe(waypoint_topic_name, 1, waypoint_topic_Callback);

ros::Subscriber vicon_subscriber = nh.subscribe(vicon_topic_name, 1, vicon_topic_Callback);

ros::Publisher cmd_vel_publisher = nh.advertise<geometry_msgs::Twist>(cmd_vel_topic_name,1);


  ros::Rate rate(loop_rate);

  while(ros::ok())
  {
   // Math stuff happens below. The actual P controller is at work below. 

		double k = 0.3;

		double vel_x = 0;
		double vel_y = 0;
		geometry_msgs::Twist output_vel;

		double error_x = waypoint_x - true_x; // calculated error x component
		double error_y = waypoint_y - true_y; // caclulated error y component
		
		double norm = sqrt(error_x*error_x + error_y * error_y); // magnitude of the 
		double unit_x = error_x/norm; // x component of unit vector
		double unit_y = error_y/norm; // y component of unit vector
		
		if(norm > 0.05){

			// proportional velocity vector
			vel_x = k * error_x;
			vel_y = k * error_y;

			// check if the computed velocity is larger than the max.
			double check_vel = sqrt(vel_x*vel_x + vel_y* vel_y);
			
			// if it is larger, multipl the unit vector of the error by the max velocity
			if(check_vel > max_velocity){
				vel_x = unit_x * max_velocity;
				vel_y = unit_y * max_velocity; // the resultant vector is at the maximum speed in the direction. 
			}

		}
		else{
			vel_x = 0;
			vel_y = 0; 

		}
	// storing the x and y velocity to be published
	output_vel.linear.x = vel_x;
	output_vel.linear.y = vel_y; 
	
	cmd_vel_publisher.publish(output_vel); 

    ros::spinOnce();
    rate.sleep();
  }
}


void waypoint_topic_Callback(const geometry_msgs::Twist::ConstPtr& msg) //TODO: MEssage type needs to be set <<
{

waypoint_x = msg -> linear.x;
waypoint_y = msg -> linear.y; 



/* NOTE: old code below from the holonomic controller. Used as reference for currnet proportional controller code. 
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
*/

}

void vicon_topic_Callback(const geometry_msgs::TransformStamped::ConstPtr& msg){ //TODO: MEssage type needs to be set <<
	
	true_x = msg -> transform.translation.x; //obtainin the data from the vicon sensor about the pose.
	true_y = msg -> transform.translation.y;





}































