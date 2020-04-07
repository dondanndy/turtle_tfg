#include <ros/ros.h>
#include <string>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

bool move_to_goal(double ptX, double ptY){
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);

	while(!ac.waitForServer(ros::Duration(5.0))){
		ROS_INFO("Iniciando server");
	}

	move_base_msgs::MoveBaseGoal goal;
	
	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	//Movimiento

	goal.target_pose.pose.position.x = ptX;
	goal.target_pose.pose.position.y = ptY;
	//goal.target_pose.pose.position.z = 0.0;

	goal.target_pose.pose.orientation.x = 0.0;
	goal.target_pose.pose.orientation.y = 0.0;
	goal.target_pose.pose.orientation.z = 0.0;
	goal.target_pose.pose.orientation.w = 1.0;

	ac.sendGoal(goal);
	ac.waitForResult();

	if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
		return true;
	} else {
		return false;
	}
}

std::string choose(std::string txt){
	std::string chose = "u";

	std::cout << txt << std::endl;

	std::cin >> chose;

	return chose;
}


//Puntos
double pt1_x = 1.0;
double pt1_y = 0.0;

int main(int argc, char** argv){
	ros::init(argc, argv, "map_navigation_node");
	ros::NodeHandle n;
	ros::spinOnce();


	bool suc;
	std::string ch;
	
	do{
	//Para salir solo hace falta escribir algo que no sea un numero.
		pt1_x = std::stod(choose("Elige el punto X"));
		pt1_y = std::stod(choose("Elige el punto Y"));
		suc = move_to_goal(pt1_x, pt1_y);

		if (suc){
			ROS_INFO("El robot ha llegado a su destino");
		} else {
			ROS_INFO("Ha habido un fallo en el movimiento");
		}
	
	} while(ch != "u");

	return 0;

}
