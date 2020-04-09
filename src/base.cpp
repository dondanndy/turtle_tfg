#include "base.h"

void Base::log_position(const Pose& pos){

    tf::TransformListener listener;
    tf::StampedTransform trans;    

    //Posicion teorica

    std::cout << "Posición teorica -----" << std::endl;

    std::cout << pos.position.x << "," << pos.position.y << std::endl;
    
    std::cout << "Yaw - " << tf::getYaw(pos.orientation) << std::endl;

    //Posicion real
    bool dato_OK = true;
    while(dato_OK){
        //Reintentamos hasta que damos con el topic.
        try{
            listener.lookupTransform("map", "base_link", ros::Time(), trans);

            std::cout << "Posición real -----" << std::endl;

            std::cout << trans.getOrigin().getX() << "," << trans.getOrigin().getY() << std::endl;
            
            double roll, pitch, yaw;
            trans.getBasis().getRPY(roll, pitch, yaw);
            std::cout << "Yaw - " << yaw << std::endl;
            dato_OK = false; //Salimos.
        
        }
        catch(tf::TransformException& ex) {
            //std::cout << "Buscando transformacion \r";
        }
    }
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

    std::cout << posiciones.size() << std::endl;
    
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
        this->log_position(pos);
    };

    ROS_INFO("Final de la trayectoria.");
}
