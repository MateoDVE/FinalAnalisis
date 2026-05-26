#pragma once
#include <vector>
#include <queue>
#include "GrafoVuelos.h"

using namespace std;

class BFS {
public:
    int contarAlcanzables(GrafoVuelos& grafo, int origenIdx, int maxEscalas) {
        int maxAristas = maxEscalas + 1;
        int n = grafo.getCantidadNodos();
        vector<Aeropuerto>& aeropuertos = grafo.getAeropuertos();

        vector<int> distancias(n, -1);
        queue<int> cola;

        cola.push(origenIdx);
        distancias[origenIdx] = 0;

        int aeropuertosAlcanzados = 0;

        while (!cola.empty()) {
            int u = cola.front();
            cola.pop();

            if (distancias[u] >= maxAristas) continue;

            for (Ruta& ruta : aeropuertos[u].getRutasSalida()) {
                int v = ruta.getIdDestino();
                if (distancias[v] == -1) {
                    distancias[v] = distancias[u] + 1;
                    cola.push(v);
                    aeropuertosAlcanzados++;
                }
            }
        }
        return aeropuertosAlcanzados;
    }
};