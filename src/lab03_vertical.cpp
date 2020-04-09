#include "base.h"

class Trayectoria_vertical : public Base {
    // Trayectoria que recorre el laboratorio de forma recta.

    std::vector<Pose> path() override {
        //TODO
        std::vector<Pose> ruta;
        Pose position;

        const int ANCHO = 3;
        const int ALTO = 4;

        bool orient = false; //Comenzamos hacia abajo.

        for (int i=ANCHO; i <= -ANCHO; i--){
            for (int j=ALTO; j <= -ALTO; j--){
                if (orient){
                    position.position.x = i;
                    position.position.y = -j;

                    position.orientation = tf::createQuaternionMsgFromYaw(1.57); //+90 grados
                } else {
                    position.position.x = i;
                    position.position.y = j;

                    position.orientation = tf::createQuaternionMsgFromYaw(-1.57); //-90 grados
                }
            ruta.push_back(position);
            }
            orient != orient;
        }
        return(ruta);
    }
};

int main(int argc, char** argv){

    Trayectoria_vertical Tray;
    Tray.run(argc, argv);

    return 0;
}
