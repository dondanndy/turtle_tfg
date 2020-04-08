#include "base.h"

class Trayectoria_vertical : public Base {
    // Trayectoria que recorre el laboratorio de forma recta.

    std::vector<Pose> path() override {
        //TODO
        std::vector<Pose> ruta;
        Pose position;

        for (int i=0; i < 3; i++){
            position.position.x = 0.0;
            position.position.y = static_cast<double>(-i);
            position.position.z = 0.0;
   
            position.orientation = tf::createQuaternionMsgFromYaw(0.0);
	/*
            position.orientation.x = 0.0;
            position.orientation.y = 0.0;
            position.orientation.z = 0.0;
            position.orientation.w = 1.0;*/

            ruta.push_back(position);
        }

        return(ruta);
    }
};

int main(int argc, char** argv){

    Trayectoria_vertical Tray;
    Tray.run(argc, argv);

    return 0;
}
