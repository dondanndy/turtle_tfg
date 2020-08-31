#include <base.h>

void log_position(const Pose& pos);
bool move_to_goal(const Pose& pos);
void update_position(const PoseCov& position);

std::string get_tag_name(){
    return("ESP");
}

std::vector<Pose> path() {
        std::vector<Pose> ruta;
        Pose position;

        const int MAX = 2;

        for(int i=0; i <= MAX; i++){

            int x = i;
            int y = i;

            position.position.x = x;
            position.position.y = y;

            position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados

            ruta.push_back(position);

            // Trayecto hacia abajo
            for(int j=i; j>-i; j--){
                y -= 1;

                position.position.x = x;
                position.position.y = y;

                position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados

                ruta.push_back(position);
            }

            // Trayecto hacia la izquierda
            for(int j=i; j>-i; j--){
                x -= 1;

                position.position.x = x;
                position.position.y = y;

                position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados

                ruta.push_back(position);
            }

            // Trayecto hacia arriba
            for(int j=i; j>-i; j--){
                y += 1;

                position.position.x = x;
                position.position.y = y;

                position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados

                ruta.push_back(position);
            }

            // Trayecto hacia la derecha
            for(int j=i; j>-i+1; j--){
                x += 1;

                position.position.x = x;
                position.position.y = y;

                position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados

                ruta.push_back(position);
            }
        }
        return(ruta);
}

