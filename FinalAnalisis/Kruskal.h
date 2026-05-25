#pragma once
#include <vector>
#include <algorithm>
#include "GrafoVuelos.h"

using namespace std;

// Estructura Union-Find (Conjuntos Disjuntos) para evitar ciclos en O(1) amortizado
class UnionFind {
private:
    vector<int> padre;
    vector<int> rango;

public:
    UnionFind(int n) {
        padre.resize(n);
        rango.resize(n, 0);
        // Al inicio, cada aeropuerto es su propio padre (están aislados)
        for (int i = 0; i < n; i++) {
            padre[i] = i;
        }
    }

    // Encuentra la raíz del conjunto con "compresión de caminos" para máxima velocidad
    int buscar(int i) {
        if (padre[i] == i) return i;
        return padre[i] = buscar(padre[i]);
    }

    // Une dos conjuntos basándose en su rango (profundidad)
    bool unir(int i, int j) {
        int raizI = buscar(i);
        int raizJ = buscar(j);

        // Si tienen la misma raíz, formarían un ciclo. No los unimos.
        if (raizI == raizJ) return false;

        // Unimos el árbol más pequeño debajo del más grande
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
        return true;
    }
};

class Kruskal {
public:
    // Función principal para obtener el MST
    // Retorna las aristas que forman la red óptima y calcula el costo total por referencia
    static vector<GrafoVuelos::AristaPonderada> encontrarMST(
        vector<GrafoVuelos::AristaPonderada>& aristas, int numNodos, double& costoTotalKm) {

        vector<GrafoVuelos::AristaPonderada> mst;
        costoTotalKm = 0.0;

        // 1. Ordenar todas las aristas de menor a mayor distancia usando una función lambda
        sort(aristas.begin(), aristas.end(),
            [](const GrafoVuelos::AristaPonderada& a, const GrafoVuelos::AristaPonderada& b) {
                return a.peso < b.peso;
            });

        // 2. Inicializar la estructura para detectar ciclos
        UnionFind uf(numNodos);

        // 3. Procesar las aristas
        for (const auto& arista : aristas) {
            // Si unir el origen y destino NO forma un ciclo...
            if (uf.unir(arista.origen, arista.destino)) {
                mst.push_back(arista); // ...agregamos la ruta a nuestra red base
                costoTotalKm += arista.peso;
            }
        }

        return mst;
    }
};