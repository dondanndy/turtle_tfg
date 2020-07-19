#include <base.h>

void log_position(const Pose& pos);
bool move_to_goal(const Pose& pos);
void update_position(const PoseCov& position);

std::string get_tag_name(){
    return("FIS");
}

std::vector<Pose> path() {
        std::vector<Pose> ruta;
        Pose position;

        //Primer punto
        position.position.x = 0.7;
        position.position.y = -0.5;
        
        position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados
        ruta.push_back(position);

        //Segundo punto
        position.position.x = 10.5;
        position.position.y = -0.5;
        
        position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados
        ruta.push_back(position);

        //Tercer punto
        position.position.x = 10.5;
        position.position.y = 10.0;
        
        position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados
        ruta.push_back(position);

        //Cuarto punto
        position.position.x = 0.7;
        position.position.y = 10.0;
        
        position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados
        ruta.push_back(position);

        return(ruta);
}