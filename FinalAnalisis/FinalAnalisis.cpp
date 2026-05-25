#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "GrafoVuelos.h"
#include "BFS.h"      // Para el Reto 1
#include "Tarjan.h"   // Para el Reto 2
#include "Dijkstra.h" // Para el Reto 3
#include "Kruskal.h"  // Para el Reto 4

using namespace std;

int main() {
    GrafoVuelos miGrafo;

    if (!miGrafo.cargarAeropuertos("airports_clean.csv") || !miGrafo.cargarRutas("routes_clean.csv")) {
        cout << "Error al cargar los archivos." << endl;
        return 1;
    }

    int totalNodos = miGrafo.getCantidadNodos();
    cout << "Total de nodos activos en la infraestructura: " << totalNodos << endl;

    // ==========================================
    // --- Reto 1 (Alcance - BFS) ---
    // ==========================================
    cout << "\n--- Reto 1: Alcance Personalizado (Max 3 escalas) ---\n";
    string iataBuscado;
    cout << "Ingrese el codigo IATA del aeropuerto origen (ej. VVI, JFK, MAD): ";
    cin >> iataBuscado;

    int origenIdx = miGrafo.obtenerIndicePorIATA(iataBuscado);
    if (origenIdx != -1) {
        // maxEscalas = 3
        int alcanzables = BFS::contarAlcanzables(miGrafo, origenIdx, 3);
        cout << ">> Desde " << miGrafo.getNombreAeropuerto(origenIdx)
            << " (" << iataBuscado << ") puedes llegar a "
            << alcanzables << " aeropuertos distintos con un maximo de 3 escalas." << endl;
    }
    else {
        cout << ">> Error: Aeropuerto con codigo IATA '" << iataBuscado << "' no encontrado." << endl;
    }

    // ==========================================
    // --- Reto 2 (Grupos y Aislamiento - Tarjan) ---
    // ==========================================
    cout << "\n--- Reto 2: Analisis de Conectividad y Aislamiento (Tarjan) ---\n";
    vector<vector<int>> grafoParaTarjan = miGrafo.obtenerListaAdyacenciaPura();
    vector<int> gruposSCC = Tarjan::encontrarSCC(grafoParaTarjan);

    int numSCC = 0;
    for (int grupo : gruposSCC) {
        if (grupo > numSCC) numSCC = grupo;
    }
    numSCC++; // Si el max es 5, hay 6 grupos (0 al 5)

    // Arreglos para filtrar los verdaderos "aislados"
    vector<bool> esAislado(numSCC, true);
    vector<int> tamanoSCC(numSCC, 0);

    for (int i = 0; i < totalNodos; i++) {
        int miGrupo = gruposSCC[i];
        tamanoSCC[miGrupo]++;

        for (int vecino : grafoParaTarjan[i]) {
            int grupoVecino = gruposSCC[vecino];
            if (miGrupo != grupoVecino) {
                // Si un aeropuerto se conecta con otro grupo, ambos grupos ya NO están aislados del mundo
                esAislado[miGrupo] = false;
                esAislado[grupoVecino] = false;
            }
        }
    }

    int islasEncontradas = 0, tamanoMaxIsla = 0;
    for (int i = 0; i < numSCC; i++) {
        // El reto dice "donde es posible viajar internamente", entonces asumimos que al menos necesitan 2 aeropuertos
        if (esAislado[i] && tamanoSCC[i] >= 2) {
            islasEncontradas++;
            if (tamanoSCC[i] > tamanoMaxIsla) tamanoMaxIsla = tamanoSCC[i];
        }
    }

    cout << ">> Grupos totalmente aislados encontrados: " << islasEncontradas << endl;
    cout << ">> El grupo aislado mas grande contiene " << tamanoMaxIsla << " aeropuertos." << endl;

    // ==========================================
    // --- Reto 3 (Diametro con Dijkstra) ---
    // ==========================================
    cout << "\n--- Reto 3: La Maxima Eficiencia (Diametro del Grafo) ---\n";
    Dijkstra::ResultadoDiametro diametro = Dijkstra::encontrarDiametro(miGrafo);

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

    // ==========================================
    // --- FASE 5: Reto 4 (Red Minima de Sudamerica - Kruskal MST) ---
    // ==========================================
    cout << "\n--- Reto 4: Red Minima de Sudamerica (Kruskal) ---\n";

    vector<GrafoVuelos::AristaPonderada> todasLasAristas = miGrafo.obtenerTodasLasAristas();
    vector<GrafoVuelos::AristaPonderada> aristasSudamerica;

    // Filtramos para quedarnos solo con las rutas internas de Sudamérica
    for (const auto& arista : todasLasAristas) {
        if (miGrafo.esSudamerica(miGrafo.getPais(arista.origen)) &&
            miGrafo.esSudamerica(miGrafo.getPais(arista.destino))) {
            aristasSudamerica.push_back(arista);
        }
    }

    double costoMST_SA = 0.0;
    // Ejecutamos Kruskal pasando únicamente las aristas de la región
    vector<GrafoVuelos::AristaPonderada> redBaseSA = Kruskal::encontrarMST(aristasSudamerica, totalNodos, costoMST_SA);

    cout << ">> La red sudamericana original tenia " << aristasSudamerica.size() << " conexiones." << endl;
    cout << ">> La Red Minima necesaria calculada requiere solo " << redBaseSA.size() << " rutas." << endl;
    cout << fixed << setprecision(2);
    cout << ">> Kilometraje total minimo para mantener a Sudamerica conectada: " << costoMST_SA << " km." << endl;

    return 0;
}