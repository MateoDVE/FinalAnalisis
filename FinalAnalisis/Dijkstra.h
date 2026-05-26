#pragma once
#include <vector>
#include <queue>
#include <limits>
#include <iostream>
#include "GrafoVuelos.h"

using namespace std;
using std::pair;

class Dijkstra {
public:
    struct ResultadoDiametro {
        int origenIdx;
        int destinoIdx;
        double distanciaMaxima;
    };

    ResultadoDiametro encontrarDiametro(GrafoVuelos& grafo) {
        int n = grafo.getCantidadNodos();
            double INFINITO = numeric_limits<double>::infinity();
            vector<Aeropuerto>& nodos = grafo.getAeropuertos();

        ResultadoDiametro diametroGlobal = { -1, -1, -1.0 };

        cout << "Calculando el Diametro de la red mundial (Dijkstra optimizado)..." << endl;

        for (int i = 0; i < n; i++) {
            if (i % 1000 == 0 && i > 0) {
                cout << "Procesando rutas desde el aeropuerto " << i << " de " << n << "..." << endl;
            }

            vector<double> distancias(n, INFINITO);
            priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

            distancias[i] = 0.0;
            pq.push({ 0.0, i });

            double maxDistanciaDesdeI = -1.0;
            int nodoMasLejanoDesdeI = -1;

            while (!pq.empty()) {
                double distActual = pq.top().first;
                int u = pq.top().second;
                pq.pop();

                if (distActual > distancias[u]) continue;

                if (distActual > maxDistanciaDesdeI && distActual != INFINITO) {
                    maxDistanciaDesdeI = distActual;
                    nodoMasLejanoDesdeI = u;
                }

                    for (Ruta& ruta : nodos[u].getRutasSalida()) {
                        int v = ruta.getIdDestino();
                        double peso = ruta.getDistanciaKm();

                    if (distancias[u] + peso < distancias[v]) {
                        distancias[v] = distancias[u] + peso;
                        pq.push({ distancias[v], v });
                    }
                }
            }

            if (maxDistanciaDesdeI > diametroGlobal.distanciaMaxima) {
                diametroGlobal.distanciaMaxima = maxDistanciaDesdeI;
                diametroGlobal.origenIdx = i;
                diametroGlobal.destinoIdx = nodoMasLejanoDesdeI;
            }
        }

        return diametroGlobal;
    }
};