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

        float min_x = 0.7;
        float min_y = -0.5;

        float max_x = 10.0;
        float max_y = 10.0;

        const int MAX_PTS_TRAY = 3;

        //Primera parte
        for (int n=0; n <= MAX_PTS_TRAY; n++){
            position.position.x = min_x + (max_x - min_x)*n/(MAX_PTS_TRAY + 1);
            position.position.y = min_y ;
            
            position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados
            ruta.push_back(position);
        }


        //Segunda parte
        for (int n=0; n <= MAX_PTS_TRAY; n++){
            position.position.x = max_x ;
            position.position.y = min_y + (max_y - min_y)*n/(MAX_PTS_TRAY + 1);
            
            position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados
            ruta.push_back(position);
        }

        //Tercera parte
        for (int n=0; n <= MAX_PTS_TRAY; n++){
            position.position.x = max_x - (max_x - min_x)*n/(MAX_PTS_TRAY + 1);
            position.position.y = max_y ;
            
            position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados
            ruta.push_back(position);
        }

        //Cuarta parte
        for (int n=0; n <= MAX_PTS_TRAY; n++){
            position.position.x = min_x ;
            position.position.y = max_y - (max_y - min_y)*n/(MAX_PTS_TRAY + 1);
            
            position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados
            ruta.push_back(position);
        }

        return(ruta);
}