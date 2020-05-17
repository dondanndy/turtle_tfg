#include <string>
#include <vector>
#include <array>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>

typedef geometry_msgs::Pose Pose;
typedef geometry_msgs::PoseWithCovarianceStamped PoseCov;

const unsigned int MAX_INT = 3;
const unsigned int REP = 1;

Pose actual_pose;
boost::array<double,36> actual_pose_covariance;

std::vector<Pose> path();

void log_position(const Pose& pos){
    bool SALIDA_TF = false;

    tf::TransformListener listener;
    tf::StampedTransform trans;    

    //Posicion teorica

    std::cout << "Posición teorica -----" << std::endl;

    std::cout << pos.position.x << "," << pos.position.y << std::endl;
    
    //std::cout << "Yaw - " << tf::getYaw(pos.orientation) << std::endl;

    //Posicion real, tf
    if (SALIDA_TF){
    bool dato_OK = true;
    while(dato_OK){
        //Reintentamos hasta que damos con el topic.
        try{
            listener.lookupTransform("map", "base_link", ros::Time(), trans);

            std::cout << "Posición tf -----" << std::endl;

            std::cout << trans.getOrigin().getX() << "," << trans.getOrigin().getY() << std::endl;
            
            double roll, pitch, yaw;
            trans.getBasis().getRPY(roll, pitch, yaw);
            std::cout << "Yaw - " << yaw << std::endl;
            dato_OK = false; //Salimos.
        
        }
        catch(tf::TransformException& ex) {
            //std::cout << "Buscando transformacion \r";
        }
    }}

    //amcl_pose

    std::cout << "Posición amcl -----" << std::endl;
    std::cout << actual_pose.position.x << "," << actual_pose.position.y << std::endl;
    std::cout << actual_pose_covariance[0] << "," << actual_pose_covariance[7] << std::endl;

    std::cout << "--------------------" << std::endl;
}

bool move_to_goal(const Pose& pos){
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);
    while(!ac.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Iniciando server");
    }
    
    move_base_msgs::MoveBaseGoal goal;
    
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose = pos;
    //Movimiento
    ac.sendGoal(goal);
    ac.waitForResult();
    return(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED);
}


void update_position(const PoseCov& position){
    actual_pose = position.pose.pose;
    actual_pose_covariance = position.pose.covariance;
}

int main(int argc, char** argv){

    ros::init(argc, argv, "map_navigation");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("amcl_pose",500, update_position);
    ros::spinOnce();
    bool suc;

    auto posiciones = path(); // Vector con las posiciones de la trayectoria.
    
    for (int i=0; i<REP; i++){
        for (const Pose& pos : posiciones){
            suc = move_to_goal(pos);
            if (!suc){
                ROS_INFO("El robot no ha llegado a su destino, reintentando");
                for (int i=0; i < MAX_INT; i++){
                    suc = move_to_goal(pos);
                    if (suc) break;
                }
            }
	    ros::spinOnce();
        log_position(pos);
        };
    };

    ROS_INFO("Final de la trayectoria.");
    return 0;
}
