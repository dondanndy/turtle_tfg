#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <chrono>
#include <thread>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>

typedef geometry_msgs::Pose Pose;
typedef geometry_msgs::PoseWithCovarianceStamped PoseCov;

const std::string ruta("/home/tb2b/catkin_ws/src/");

const unsigned int MAX_PTS_SENSOR = 50;
const unsigned int MAX_INT = 3;
const unsigned int REP = 1;
const bool SALIDA_TF = false;

// Posicion del robot.
Pose actual_pose;
boost::array<double,36> actual_pose_covariance;

std::vector<Pose> path();
std::string get_tag_name();

std::string get_sensor_data(){
    std::ifstream consola;
	std::ofstream file; 
	std::string linea;
	unsigned int num_datos = 0;

	while (num_datos < MAX_PTS_SENSOR){
        
        consola.open("/dev/ttyACM0");

        std::getline(consola,linea);

		if (linea.size() > 115 && linea.size() < 180) {
            // A veces se vuelve un poco loco al leer así que no guardaremos los datos que 
            // por estar con menos caracteres no nos sirven.
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

			file.open(ruta + "turtle_tfg/src/datos/sensor/" + tag + ".txt", std::ios_base::app);
			file << linea << "\n";
			file.close();
		
            num_datos++;
        }
        
        consola.close();
	}

        //TODO
    return("0.0\n");
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

void log_position(const std::string& file_name, const Pose& pos){
    std::string log_pos; //Posicion a escribir en el archivo


    //Posicion teorica

    //std::cout << "Posición teorica -----" << std::endl;

    //std::cout << pos.position.x << "," << pos.position.y << std::endl; 
    //std::cout << "Yaw - " << tf::getYaw(pos.orientation) << std::endl;

    log_pos = std::to_string(pos.position.x) + "\t" + std::to_string(pos.position.y) + "\t"; // Log de la posicion teorica

    //Posicion real, tf
    if (SALIDA_TF){
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
    }}

    //amcl_pose

    std::cout << "Posición amcl -----" << std::endl;
    std::cout << actual_pose.position.x << "," << actual_pose.position.y << std::endl;
    std::cout << actual_pose_covariance[0] << "," << actual_pose_covariance[7] << std::endl;

    std::cout << "--------------------" << std::endl;

    log_pos = log_pos
              + std::to_string(actual_pose.position.x) + "\t"
              + std::to_string(actual_pose_covariance[0]) + "\t"
              + std::to_string(actual_pose.position.y) + "\t"
              + std::to_string(actual_pose_covariance[7]) + "\t"
              + get_sensor_data(); // Log de la posicion de amcl

    // Al archivo
    std::ofstream file(file_name, std::ios_base::app);
    file << log_pos;
    file.close();
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

    std::string date = get_date();

    std::string file = "/home/dondanndy/catkin_ws/src/turtle_tfg/datos/" + get_date() + ".txt"; // Nombre del rchivo para guardar los puntos.

    auto posiciones = path(); // Vector con las posiciones de la trayectoria.
    
    for (int k=1; k<=REP; k++){
        file = ruta + "turtle_tfg/src/datos/" 
               + get_tag_name() + "-" +  date + "-" + std::to_string(k) + ".txt"; 

        for (int i=0; i < posiciones.size(); i++){
            suc = move_to_goal(posiciones[i]);
            if (!suc){
                ROS_INFO("El robot no ha llegado a su destino, reintentando");
                for (int j=0; j < MAX_INT; j++){
                    suc = move_to_goal(posiciones[i]);
                    if (suc) break;
                }
            }
	    ros::spinOnce();

        std::string tag = get_tag_name() + std::to_string(i) + date + "-" + std::to_string(k); //Tag de cada punto.
        log_position(file, posiciones[i], tag);
        };
    };

    ROS_INFO("Final de la trayectoria.");
    return 0;
}
