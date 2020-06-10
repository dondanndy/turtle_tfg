#include <string>
#include <vector>
#include <array>
#include <fstream>
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
const bool USAR_TF = false;


// Posicion del robot.
Pose actual_pose;
boost::array<double,36> actual_pose_covariance;

std::vector<Pose> path();

std::string get_sensor_data(){
    //TODO
    return("1.0\t2.0\t95\n");
}

std::string get_date(){
    /*
        Devuelve fecha y hora.
     */

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    //Fecha a texto
    char text[50];
    std::strftime(text, sizeof(text), "%Y-%m-%d--%Hh%Mm", std::localtime(&now));

    std::string string_return(text); // Devolvemos std::string en vez de char*
    return(string_return);
}

void log_position(const std::ofstream& file, const Pose& pos){
    std:string pos; //Posicion a escribir en el archivo


    //Posicion teorica

    std::cout << "Posición teorica -----" << std::endl;

    std::cout << pos.position.x << "," << pos.position.y << std::endl;
    std::cout << "Yaw - " << tf::getYaw(pos.orientation) << std::endl;

    pos = to_string(pos.position.x) + "\t" + to_string(pos.position.y) + "\t" // Log de la posicion teorica

    //Posicion real, tf
    if (USAR_TF){
        // En un principio no vamos a usar tf para conseguir la posicion así que lo esdcondemos detras de un bool en false.

        tf::TransformListener listener;
        tf::StampedTransform trans;

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
        }
    }


    //amcl_pose

    // DEBUG
    // std::cout << "Posición amcl -----" << std::endl;
    // std::cout << actual_pose.position.x << "," << actual_pose.position.y << std::endl;

    pos = pos + to_string(actual_pose.position.x) + "\t" + to_string(actual_pose.position.y) + "\t" + get_sensor_data(); // Log de la posicion de amcl

    // Al archivo
    file << pos;
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
    // Callback para actualizar la posicion del robot al moverse.

    actual_pose = position.pose.pose;
    actual_pose_covariance = position.pose.covariance;
}

int main(int argc, char** argv){

    ros::init(argc, argv, "map_navigation");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("amcl_pose",500, update_position);
    ros::spinOnce();
    bool suc;

    std::ofstream file("datos/" + get_date() + ".txt"); // Archivo para guardar los puntos.

    auto posiciones = path(); // Vector con las posiciones de la trayectoria.
    
    for (int i=0; i<REP; i++){
        for (const Pose& pos : posiciones){
            suc = move_to_goal(pos);
            if (!suc){
                // A veces en el simulador el robot se quedaba dando vueltas, intentamos llegar al objetivo
                // varias veces si no llega la primera.
                ROS_INFO("El robot no ha llegado a su destino, reintentando");
                for (int i=0; i < MAX_INT; i++){
                    suc = move_to_goal(pos);
                    if (suc) break;
                }
            }
	    ros::spinOnce();
        log_position(file, pos);
        };
    };

    // Todo acabado.
    ROS_INFO("Final de la trayectoria.");
    file.close();

    return 0;
}


