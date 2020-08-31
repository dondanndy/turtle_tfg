#include "base.h"
// Trayectoria que recorre el laboratorio de forma aleatoria.

std::string get_tag_name(){
    return("RAND");
}

std::vector<Pose> path(){
    std::vector<Pose> ruta;
    Pose position;

    const unsigned int ANCHO = 2;
    const unsigned int ALTO = 3;

    //Distribuciones pseudo-aleatorias
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Generamos todos los datos posibles.
    for (int i = -ANCHO; i <= ANCHO; i++){
        for (int j = -ALTO; j <= ALTO; j++){
            position.position.x = i;
            position.position.y = j;
            position.orientation = tf::createQuaternionMsgFromYaw(0);

            ruta.push_back(position);
        }
    }

    // Mezcla aleatoria del vector.
    std::shuffle(ruta.begin(), ruta.end(), g);

    return(ruta);
}
