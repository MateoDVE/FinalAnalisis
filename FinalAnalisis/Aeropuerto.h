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
    Aeropuerto(int id, const string& nom, double lat, double lon)
        : idReal(id), nombre(nom), latitud(lat), longitud(lon) {
    }

    int getIdReal() const { return idReal; }
    string getNombre() const { return nombre; }
    double getLatitud() const { return latitud; }
    double getLongitud() const { return longitud; }
    const vector<Ruta>& getRutasSalida() const { return rutasSalida; }

    void agregarRuta(int idDestino, double distanciaKm) {
        rutasSalida.push_back(Ruta(idDestino, distanciaKm));
    }
};