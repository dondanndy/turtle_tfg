#include "base.h"

void Base::log_position(const Pose& pos){
    //TODO
    ROS_INFO("Log");
}

bool Base::move_to_goal(const Pose& pos){
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

void Base::run(int argc, char** argv){
    ros::init(argc, argv, "map_navigation");
    ros::NodeHandle n;
    ros::spinOnce();
    bool suc;

    auto posiciones = this->path(); // Vector con las posiciones de la trayectoria.
    
    for (const Pose& pos : posiciones){
        suc = move_to_goal(pos);
        if (!suc){
            ROS_INFO("El robot no ha llegado a su destino, reintentando");
            for (int i=0; i < MAX_INT; i++){
                suc = move_to_goal(pos);
                if (suc) break;
            }
        }
        this->log_position(pos);
    };
    ROS_INFO("Final de la trayectoria.");
}
