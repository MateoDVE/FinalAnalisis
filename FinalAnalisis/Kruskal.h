#pragma once
#include <vector>
#include <algorithm>
#include "GrafoVuelos.h"

using namespace std;

class UnionFind {
private:
    vector<int> padre;
    vector<int> rango;

public:
    UnionFind(int n) {
        padre.resize(n);
        rango.resize(n, 0);
        for (int i = 0; i < n; ++i) {
            padre[i] = i;
        }
    }

    int buscar(int i) {
        if (padre[i] == i) {
            return i;
        }
        return padre[i] = buscar(padre[i]);
    }

    void unir(int i, int j) {
        int raizI = buscar(i);
        int raizJ = buscar(j);

        if (raizI != raizJ) {
            if (rango[raizI] < rango[raizJ]) {
                padre[raizI] = raizJ;
            }
            else if (rango[raizI] > rango[raizJ]) {
                padre[raizJ] = raizI;
            }
            else {
                padre[raizJ] = raizI;
                rango[raizI]++;
            }
        }
    }
};

class Kruskal {
public:
    vector<GrafoVuelos::AristaPonderada> encontrarMST(
        vector<GrafoVuelos::AristaPonderada>& aristas,
        int totalNodos,
        double& costoTotalKm)
    {
        vector<GrafoVuelos::AristaPonderada> mst;
        costoTotalKm = 0.0;

        sort(aristas.begin(), aristas.end(),
            [](const GrafoVuelos::AristaPonderada& a, const GrafoVuelos::AristaPonderada& b) {
                return a.peso < b.peso;
            });

        UnionFind uf(totalNodos);

        for (const auto& arista : aristas) {
            int origen = arista.origen;
            int destino = arista.destino;

            // Si find(origen) != find(destino) -> están en diferentes grupos
            if (uf.buscar(origen) != uf.buscar(destino)) {
                // No hay ciclo, la agregamos a nuestra Red Mínima
                mst.push_back(arista);
                costoTotalKm += arista.peso;

                // Unimos los grupos
                uf.unir(origen, destino);
            }
        }

        return mst;
    }
};