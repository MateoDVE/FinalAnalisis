#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "GrafoVuelos.h"
#include "BFS.h"
#include "Tarjan.h"
#include "Dijkstra.h"
#include "Kruskal.h"

using namespace std;

int main() {
    GrafoVuelos miGrafo;

    if (!miGrafo.cargarAeropuertos("airports_clean.csv") || !miGrafo.cargarRutas("routes_clean.csv")) {
        cout << "Error al cargar los archivos." << endl;
        return 1;
    }

    int totalNodos = miGrafo.getCantidadNodos();
    cout << "Total de nodos activos en la infraestructura: " << totalNodos << endl;
    //Reto 1
    cout << "\n--- Reto 1: Alcance Personalizado (Max 3 escalas) ---\n";
    string iataBuscado;
    cout << "Ingrese el codigo IATA del aeropuerto origen (ej. VVI, JFK, MAD): ";
    cin >> iataBuscado;

    int origenIdx = miGrafo.obtenerIndicePorIATA(iataBuscado);
    if (origenIdx != -1) {
        BFS bfs;
        int alcanzables = bfs.contarAlcanzables(miGrafo, origenIdx, 3);
        cout << ">> Desde " << miGrafo.getNombreAeropuerto(origenIdx)
            << " (" << iataBuscado << ") puedes llegar a "
            << alcanzables << " aeropuertos distintos con un maximo de 3 escalas." << endl;
    }
    else {
        cout << ">> Error: Aeropuerto con codigo IATA '" << iataBuscado << "' no encontrado." << endl;
    }

    //Reto 2
    cout << "\n--- Reto 2: Analisis de Conectividad y Aislamiento (Tarjan) ---\n";
    vector<vector<int>> grafoParaTarjan = miGrafo.obtenerListaAdyacenciaPura();
    Tarjan tarjan;
    vector<int> gruposSCC = tarjan.encontrarSCC(grafoParaTarjan);

    int numSCC = 0;
    for (int grupo : gruposSCC) {
        if (grupo > numSCC) numSCC = grupo;
    }
    numSCC++;

    vector<bool> esAislado(numSCC, true);
    vector<int> tamanoSCC(numSCC, 0);

    for (int i = 0; i < totalNodos; i++) {
        int miGrupo = gruposSCC[i];
        tamanoSCC[miGrupo]++;

        for (int vecino : grafoParaTarjan[i]) {
            int grupoVecino = gruposSCC[vecino];
            if (miGrupo != grupoVecino) {
                esAislado[miGrupo] = false;
                esAislado[grupoVecino] = false;
            }
        }
    }

    int islasEncontradas = 0, tamanoMaxIsla = 0;
    for (int i = 0; i < numSCC; i++) {
        if (esAislado[i] && tamanoSCC[i] >= 2) {
            islasEncontradas++;
            if (tamanoSCC[i] > tamanoMaxIsla) tamanoMaxIsla = tamanoSCC[i];
        }
    }

    cout << ">> Grupos totalmente aislados encontrados: " << islasEncontradas << endl;
    cout << ">> El grupo aislado mas grande contiene " << tamanoMaxIsla << " aeropuertos." << endl;

    //Reto 3
    cout << "\n--- Reto 3: La Maxima Eficiencia (Diametro del Grafo) ---\n";
    Dijkstra dijkstra;
    Dijkstra::ResultadoDiametro diametro = dijkstra.encontrarDiametro(miGrafo);

    if (diametro.origenIdx != -1 && diametro.destinoIdx != -1) {
        cout << "\n=============================================" << endl;
        cout << "RESULTADO COMPLETO RETO 3:" << endl;
        cout << "El diametro de la red mundial de vuelos es:" << endl;
        cout << "Origen:  " << miGrafo.getNombreAeropuerto(diametro.origenIdx) << endl;
        cout << "Destino: " << miGrafo.getNombreAeropuerto(diametro.destinoIdx) << endl;
        cout << fixed << setprecision(2);
        cout << "Distancia Maxima Encontrada: " << diametro.distanciaMaxima << " km" << endl;
        cout << "=============================================" << endl;
    }

    //Reto 4
    cout << "\n--- Reto 4: Red Minima de Sudamerica (Kruskal) ---\n";

    vector<GrafoVuelos::AristaPonderada> todasLasAristas = miGrafo.obtenerTodasLasAristas();
    vector<GrafoVuelos::AristaPonderada> aristasSudamerica;

    for (GrafoVuelos::AristaPonderada& arista : todasLasAristas) {
        if (miGrafo.esSudamerica(miGrafo.getPais(arista.origen)) &&
            miGrafo.esSudamerica(miGrafo.getPais(arista.destino))) {
            aristasSudamerica.push_back(arista);
        }
    }

    double costoMST_SA = 0.0;
    Kruskal kruskal;
    vector<GrafoVuelos::AristaPonderada> redBaseSA = kruskal.encontrarMST(aristasSudamerica, totalNodos, costoMST_SA);

    cout << ">> La red sudamericana original tenia " << aristasSudamerica.size() << " conexiones." << endl;
    cout << ">> La Red Minima necesaria calculada requiere solo " << redBaseSA.size() << " rutas." << endl;
    cout << fixed << setprecision(2);
    cout << ">> Kilometraje total minimo para mantener a Sudamerica conectada: " << costoMST_SA << " km." << endl;

    return 0;
}