#include "base.h"

class Trayectoria_aleatoria : public Base {
    // Trayectoria que recorre el laboratorio de forma recta.

    std::vector<Pose> path() override {
        std::vector<Pose> ruta;
        Pose position;

        const unsigned int ANCHO = 2;
        const unsigned int ALTO = 3;
        const unsigned int PUNTOS = 30;

        //Distribuciones pseudo-aleatorias
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::uniform_int_distribution<> distr_ancho(-ANCHO, ANCHO);
        std::uniform_int_distribution<> distr_ALTO(-ALTO, ALTO);

        for (int i=0; i < PUNTOS; i++){
            position.position.x = dis_ancho(gen);
            position.position.y = dis_alto(gen);
            position.orientation = tf::createQuaternionMsgFromYaw(0);

            ruta.push_back(position);
        }

        return(ruta);
    }
};

int main(int argc, char** argv){

    Trayectoria_aleatoria Tray;
    Tray.run(argc, argv);

    return 0;
}
