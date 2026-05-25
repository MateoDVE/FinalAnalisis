#pragma once
#include <vector>
#include <algorithm>
#include "GrafoVuelos.h"

using namespace std;

// Estructura auxiliar para manejar los conjuntos (Union-Find)
class UnionFind {
private:
    vector<int> padre;
    vector<int> rango; // O "cont" como lo menciona tu teoría, para balancear el árbol

public:
    UnionFind(int n) {
        padre.resize(n);
        rango.resize(n, 0);
        // Inicializar: cada nodo es padre de sí mismo (grupos individuales)
        for (int i = 0; i < n; ++i) {
            padre[i] = i;
        }
    }

    // Encuentra la raíz del grupo al que pertenece el nodo 'i'
    int buscar(int i) {
        if (padre[i] == i) {
            return i;
        }
        // Compresión de caminos: hacemos que el nodo apunte directamente a la raíz
        return padre[i] = buscar(padre[i]);
    }

    // Une los grupos de los nodos 'i' y 'j'
    void unir(int i, int j) {
        int raizI = buscar(i);
        int raizJ = buscar(j);

        if (raizI != raizJ) {
            // Unir el árbol más pequeño debajo del más grande (Unión por rango/tamaño)
            if (rango[raizI] < rango[raizJ]) {
                padre[raizI] = raizJ;
            }
            else if (rango[raizI] > rango[raizJ]) {
                padre[raizJ] = raizI;
            }
            else {
                padre[raizJ] = raizI;
                rango[raizI]++; // Si son iguales, elegimos uno y le subimos el rango
            }
        }
    }
};

class Kruskal {
public:
    // Retorna la lista de aristas que conforman el MST y guarda el costo en costoTotalKm
    static vector<GrafoVuelos::AristaPonderada> encontrarMST(
        vector<GrafoVuelos::AristaPonderada>& aristas,
        int totalNodos,
        double& costoTotalKm)
    {
        vector<GrafoVuelos::AristaPonderada> mst;
        costoTotalKm = 0.0;

        // 1. Ordenar TODAS las aristas de menor a mayor peso (distancia)
        sort(aristas.begin(), aristas.end(),
            [](const GrafoVuelos::AristaPonderada& a, const GrafoVuelos::AristaPonderada& b) {
                return a.peso < b.peso;
            });

        // 2. Inicializar Union Find -> cada nodo en su propio grupo
        UnionFind uf(totalNodos);

        // 3. Para cada arista (origen, destino, peso) en orden ascendente
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