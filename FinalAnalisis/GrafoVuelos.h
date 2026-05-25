#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <limits> 
#include "Aeropuerto.h"

using namespace std;

class GrafoVuelos {
private:
    vector<Aeropuerto> aeropuertos;
    unordered_map<int, int> idRealAIndiceInterno;

    // --- NUEVAS ESTRUCTURAS DE CONTROL PARA RETOS 1 Y 4 ---
    vector<string> paisesNodos;             // Guarda el país de cada aeropuerto indexado internamente
    unordered_map<string, int> iataAIndice; // Mapea el código de 3 letras IATA al índice interno

    double calcularDistanciaGeo(double lat1, double lon1, double lat2, double lon2) const {
        const double R = 6371.0;
        const double PI = 3.141592653589793;
        double phi1 = lat1 * PI / 180.0;
        double phi2 = lat2 * PI / 180.0;
        double dPhi = (lat2 - lat1) * PI / 180.0;
        double dLam = (lon2 - lon1) * PI / 180.0;
        double a = sin(dPhi / 2) * sin(dPhi / 2) +
            cos(phi1) * cos(phi2) *
            sin(dLam / 2) * sin(dLam / 2);
        return R * 2 * atan2(sqrt(a), sqrt(1 - a));
    }

    bool stringAInt(const string& str, int& resultado) const {
        stringstream ss(str);
        return (ss >> resultado) ? true : false;
    }

    bool stringADouble(const string& str, double& resultado) const {
        stringstream ss(str);
        return (ss >> resultado) ? true : false;
    }

public:
    GrafoVuelos() = default;

    struct AristaPonderada {
        int origen;
        int destino;
        double peso;
    };

    bool cargarAeropuertos(const string& rutaArchivo) {
        ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            cerr << "Error al abrir: " << rutaArchivo << endl;
            return false;
        }

        string linea;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;

            stringstream ss(linea);
            string campo;
            vector<string> columnas;

            while (getline(ss, campo, ',')) {
                columnas.push_back(campo);
            }

            if (columnas.size() < 8) continue;

            int idReal;
            double latitud, longitud;

            if (!stringAInt(columnas[0], idReal) ||
                !stringADouble(columnas[6], latitud) ||
                !stringADouble(columnas[7], longitud)) {
                continue;
            }

            string nombre = columnas[1];
            string pais = columnas[3]; // Índice 3: País de origen
            string iata = columnas[4]; // Índice 4: Código IATA de 3 letras

            int indiceInterno = aeropuertos.size();
            aeropuertos.push_back(Aeropuerto(idReal, nombre, latitud, longitud));
            idRealAIndiceInterno[idReal] = indiceInterno;

            // --- ALMACENAMIENTO DE NUEVOS ATRIBUTOS ---
            paisesNodos.push_back(pais);
            if (iata != "\\N" && !iata.empty()) {
                iataAIndice[iata] = indiceInterno;
            }
        }
        archivo.close();
        return true;
    }

    bool cargarRutas(const string& rutaArchivo) {
        ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            cerr << "Error al abrir: " << rutaArchivo << endl;
            return false;
        }

        string linea;
        int rutasCreadas = 0;

        while (getline(archivo, linea)) {
            if (linea.empty()) continue;

            stringstream ss(linea);
            string campo;
            vector<string> columnas;

            while (getline(ss, campo, ',')) {
                columnas.push_back(campo);
            }

            if (columnas.size() < 6 || columnas[3].empty() || columnas[5].empty()) continue;

            int idRealOrigen, idRealDestino;

            if (!stringAInt(columnas[3], idRealOrigen) ||
                !stringAInt(columnas[5], idRealDestino)) {
                continue;
            }

            if (idRealAIndiceInterno.count(idRealOrigen) && idRealAIndiceInterno.count(idRealDestino)) {
                int idxOrigen = idRealAIndiceInterno[idRealOrigen];
                int idxDestino = idRealAIndiceInterno[idRealDestino];

                const Aeropuerto& orig = aeropuertos[idxOrigen];
                const Aeropuerto& dest = aeropuertos[idxDestino];

                double distancia = calcularDistanciaGeo(orig.getLatitud(), orig.getLongitud(),
                    dest.getLatitud(), dest.getLongitud());

                aeropuertos[idxOrigen].agregarRuta(idxDestino, distancia);
                rutasCreadas++;
            }
        }
        archivo.close();
        cout << "Se cargaron los datos y se crearon " << rutasCreadas << " rutas dirigidas.\n";
        return true;
    }

    vector<vector<int>> obtenerListaAdyacenciaPura() const {
        vector<vector<int>> g(aeropuertos.size());
        for (size_t i = 0; i < aeropuertos.size(); ++i) {
            for (const auto& ruta : aeropuertos[i].getRutasSalida()) {
                g[i].push_back(ruta.getIdDestino());
            }
        }
        return g;
    }

    vector<AristaPonderada> obtenerTodasLasAristas() const {
        vector<AristaPonderada> aristas;
        for (size_t i = 0; i < aeropuertos.size(); ++i) {
            for (const auto& ruta : aeropuertos[i].getRutasSalida()) {
                aristas.push_back({ (int)i, ruta.getIdDestino(), ruta.getDistanciaKm() });
            }
        }
        return aristas;
    }

    size_t getCantidadNodos() const { return aeropuertos.size(); }

    const vector<Aeropuerto>& getAeropuertos() const { return aeropuertos; }

    string getNombreAeropuerto(int indiceInterno) const {
        if (indiceInterno >= 0 && indiceInterno < (int)aeropuertos.size()) {
            return aeropuertos[indiceInterno].getNombre();
        }
        return "Desconocido";
    }

    // --- NUEVOS MÉTODOS PÚBLICOS DE ACCESO ---

    // Retorna el índice interno a partir del código IATA (Reto 1)
    int obtenerIndicePorIATA(const string& iata) const {
        if (iataAIndice.count(iata)) return iataAIndice.at(iata);
        return -1;
    }

    // Retorna el país asignado al nodo interno (Reto 4)
    string getPais(int indiceInterno) const {
        if (indiceInterno >= 0 && indiceInterno < (int)paisesNodos.size()) {
            return paisesNodos[indiceInterno];
        }
        return "Desconocido";
    }

    // Determina si un país pertenece a Sudamérica para el filtrado del MST (Reto 4)
    bool esSudamerica(const string& pais) const {
        return (pais == "Argentina" || pais == "Bolivia" || pais == "Brazil" ||
            pais == "Chile" || pais == "Colombia" || pais == "Ecuador" ||
            pais == "Guyana" || pais == "Paraguay" || pais == "Peru" ||
            pais == "Suriname" || pais == "Uruguay" || pais == "Venezuela" ||
            pais == "French Guiana" || pais == "Falkland Islands");
    }
};