#include "base.h"

std::vector<Pose> path(){
        std::vector<Pose> ruta;
        Pose position;

        const int ANCHO = 2;
        const int ALTO = 3;

        bool orient = false; //Comenzamos hacia abajo.

        for (int i=ANCHO; i >= -ANCHO; i--){
            for (int j=ALTO; j >= -ALTO; j--){
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
        orient = !orient;
    }
    return(ruta);
}
