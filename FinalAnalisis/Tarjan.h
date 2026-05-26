#pragma once
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Tarjan {
private:
    void dfs(int nodoActual, vector<vector<int>>& g, vector<int>& ids,
        vector<int>& low, vector<bool>& enPila, stack<int>& pila,
        int& id, int& contSCC, vector<int>& vecSCC) {

        ids[nodoActual] = id;
        low[nodoActual] = id;
        id++;

        pila.push(nodoActual);
        enPila[nodoActual] = true;

        for (int vecino : g[nodoActual]) {
            if (ids[vecino] == -1) {
                dfs(vecino, g, ids, low, enPila, pila, id, contSCC, vecSCC);
                low[nodoActual] = min(low[nodoActual], low[vecino]);
            }
            else if (enPila[vecino]) {
                low[nodoActual] = min(low[nodoActual], ids[vecino]);
            }
        }

        if (low[nodoActual] == ids[nodoActual]) {
            int nodoSacado;
            do {
                nodoSacado = pila.top();
                pila.pop();
                enPila[nodoSacado] = false;
                vecSCC[nodoSacado] = contSCC;
            } while (nodoSacado != nodoActual);

            contSCC++;
        }
    }

public:
    vector<int> encontrarSCC(vector<vector<int>>& g) {
        int n = g.size();
        vector<int> ids(n, -1);
        vector<int> low(n, 0);
        vector<bool> enPila(n, false);
        vector<int> vecSCC(n, -1);

        stack<int> pila;
        int id = 0;
        int contSCC = 0;

        for (int i = 0; i < n; i++) {
            if (ids[i] == -1) {
                dfs(i, g, ids, low, enPila, pila, id, contSCC, vecSCC);
            }
        }

        return vecSCC;
    }
};