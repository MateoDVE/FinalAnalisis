#include <iostream>
#include <iomanip>
#include <vector>
#include "GrafoVuelos.h"
#include "Tarjan.h"
#include "Kruskal.h"
#include "Dijkstra.h"

using namespace std;

int main() {
    GrafoVuelos miGrafo;

    if (!miGrafo.cargarAeropuertos("airports_clean.csv") || !miGrafo.cargarRutas("routes_clean.csv")) {
        cout << "Error al cargar los archivos." << endl;
        return 1;
    }

    int totalNodos = miGrafo.getCantidadNodos();
    cout << "Total de nodos activos en la infraestructura: " << totalNodos << endl;

    // --- FASE 2: Reto 1 (Tarjan) ---
    cout << "\n--- Reto 1: Analisis de Conectividad (Tarjan) ---\n";
    vector<vector<int>> grafoParaTarjan = miGrafo.obtenerListaAdyacenciaPura();
    vector<int> gruposSCC = Tarjan::encontrarSCC(grafoParaTarjan);
    int maxSCC = 0;
    for (int grupo : gruposSCC) {
        if (grupo > maxSCC) maxSCC = grupo;
    }
    cout << "Se encontraron " << (maxSCC + 1) << " Componentes Fuertemente Conexos." << endl;

    // --- FASE 3: Reto 2 (Kruskal MST) ---
    cout << "\n--- Reto 2: Red Base Optima (Kruskal MST) ---\n";
    vector<GrafoVuelos::AristaPonderada> todasLasAristas = miGrafo.obtenerTodasLasAristas();
    double costoTotalKm = 0.0;
    vector<GrafoVuelos::AristaPonderada> redBaseOptima = Kruskal::encontrarMST(todasLasAristas, totalNodos, costoTotalKm);
    cout << fixed << setprecision(2);
    cout << "La Red Base Optima se formo con " << redBaseOptima.size() << " rutas (aristas)." << endl;
    cout << "Distancia total de la infraestructura base: " << costoTotalKm << " km." << endl;

    // --- FASE 4: Reto 3 (Diámetro con Dijkstra) ---
    cout << "\n--- Reto 3: La Maxima Eficiencia (Diametro del Grafo) ---\n";
    Dijkstra::ResultadoDiametro diametro = Dijkstra::encontrarDiametro(miGrafo);

    if (diametro.origenIdx != -1 && diametro.destinoIdx != -1) {
        cout << "\n=============================================" << endl;
        cout << "RESULTADO COMPLETO RETO 3:" << endl;
        cout << "El diametro de la red mundial de vuelos es entre:" << endl;
        cout << "Origen:  " << miGrafo.getNombreAeropuerto(diametro.origenIdx) << endl;
        cout << "Destino: " << miGrafo.getNombreAeropuerto(diametro.destinoIdx) << endl;
        cout << "Distancia Máxima Encontrada: " << diametro.distanciaMaxima << " km" << endl;
        cout << "=============================================" << endl;
    }

    return 0;
}