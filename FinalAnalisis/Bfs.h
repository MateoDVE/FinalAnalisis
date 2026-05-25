#pragma once
#include <vector>
#include <queue>
#include "GrafoVuelos.h"

using namespace std;

class BFS {
public:
    static int contarAlcanzables(const GrafoVuelos& grafo, int origenIdx, int maxEscalas) {
        // 0 escalas = 1 arista de distancia (vuelo directo)
        // 3 escalas = 4 aristas de distancia máxima
        int maxAristas = maxEscalas + 1;
        int n = grafo.getCantidadNodos();
        const auto& aeropuertos = grafo.getAeropuertos();

        vector<int> distancias(n, -1);
        queue<int> cola;

        cola.push(origenIdx);
        distancias[origenIdx] = 0;

        int aeropuertosAlcanzados = 0;

        while (!cola.empty()) {
            int u = cola.front();
            cola.pop();

            // Si ya llegamos al límite de escalas, no expandimos más este camino
            if (distancias[u] >= maxAristas) continue;

            for (const auto& ruta : aeropuertos[u].getRutasSalida()) {
                int v = ruta.getIdDestino();
                if (distancias[v] == -1) { // Si no ha sido visitado
                    distancias[v] = distancias[u] + 1;
                    cola.push(v);
                    aeropuertosAlcanzados++;
                }
            }
        }
        return aeropuertosAlcanzados;
    }
};