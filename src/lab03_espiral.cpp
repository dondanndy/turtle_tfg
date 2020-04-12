#include "base.h"

class Trayectoria_vertical : public Base {
    // Trayectoria que recorre el laboratorio de forma recta.

    std::vector<Pose> path() override {
        std::vector<Pose> ruta;
        Pose position;

        const int MAX = 2;

        for(int i=0; i <= MAX; i++){
            x = i;
            y = i;

            position.position.x = x;
            position.position.y = y;

            position.orientation = tf::createQuaternionMsgFromYaw(-1.57); //-90 grados

            ruta.push_back(position);

            // Trayecto hacia abajo
            for(int j=i; j<-i; j--){
                y -= 1;

                position.position.x = x;
                position.position.y = y;

                position.orientation = tf::createQuaternionMsgFromYaw(-1.57); //-90 grados

                ruta.push_back(position);
            }

            // Trayecto hacia la izquierda
            for(int j=i; j<-i; j--){
                x -= 1;

                position.position.x = x;
                position.position.y = y;

                position.orientation = tf::createQuaternionMsgFromYaw(3.14); //-180 grados

                ruta.push_back(position);
            }

            // Trayecto hacia arriba
            for(int j=i; j<-i; j--){
                y += 1;

                position.position.x = x;
                position.position.y = y;

                position.orientation = tf::createQuaternionMsgFromYaw(1.57); //-90 grados

                ruta.push_back(position);
            }

            // Trayecto hacia la derecha
            for(int j=i; j<-i+1; j--){
                x += 1;

                position.position.x = x;
                position.position.y = y;

                position.orientation = tf::createQuaternionMsgFromYaw(0); //0 grados

                ruta.push_back(position);
            }
        }
        return(ruta);
    }
};

int main(int argc, char** argv){

    Trayectoria_vertical Tray;
    Tray.run(argc, argv);

    return 0;
}
