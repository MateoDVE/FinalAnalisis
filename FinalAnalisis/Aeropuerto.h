#pragma once
#include <string>
#include <vector>
#include "Ruta.h"

using namespace std;

class Aeropuerto {
private:
    int idReal;
    string nombre;
    double latitud;
    double longitud;
    vector<Ruta> rutasSalida;

public:
    Aeropuerto(int id, string nom, double lat, double lon)
        : idReal(id), nombre(nom), latitud(lat), longitud(lon) {
    }

    int getIdReal() { return idReal; }
    string getNombre() { return nombre; }
    double getLatitud() { return latitud; }
    double getLongitud() { return longitud; }
    vector<Ruta>& getRutasSalida() { return rutasSalida; }

    void agregarRuta(int idDestino, double distanciaKm) {
        rutasSalida.push_back(Ruta(idDestino, distanciaKm));
    }
};