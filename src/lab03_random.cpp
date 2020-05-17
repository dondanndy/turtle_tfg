#include "base.h"
// Trayectoria que recorre el laboratorio de forma recta.

    std::vector<Pose> path(){
        std::vector<Pose> ruta;
        Pose position;

        const unsigned int ANCHO = 2;
        const unsigned int ALTO = 3;
        const unsigned int PUNTOS = 30;

        //Distribuciones pseudo-aleatorias
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::uniform_int_distribution<> distr_ancho(-ANCHO, ANCHO);
        std::uniform_int_distribution<> distr_alto(-ALTO, ALTO);

        for (int i=0; i < PUNTOS; i++){
            position.position.x = distr_ancho(gen);
            position.position.y = distr_alto(gen);
            position.orientation = tf::createQuaternionMsgFromYaw(0);

            ruta.push_back(position);
        }

    return(ruta);
}
