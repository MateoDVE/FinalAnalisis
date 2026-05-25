#include <iostream>
using namespace std;

class Ruta {
private:
    int idDestino;
    double distanciaKm;

public:
    Ruta(int destino, double distancia) : idDestino(destino), distanciaKm(distancia) {}

    int getIdDestino() const { return idDestino; }
    double getDistanciaKm() const { return distanciaKm; }
};