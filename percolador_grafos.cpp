#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Clase para representar un grafo
class Grafo {
public:
    int numNodos;
    vector<vector<int>> adyacencia;
    vector<bool> verticesEliminados;  // Para marcar qué nodos han sido eliminados

    Grafo(int n) : numNodos(n), adyacencia(n), verticesEliminados(n, false) {}

    // Método para agregar una arista al grafo
    void agregarArista(int u, int v) {
        adyacencia[u].push_back(v);
        adyacencia[v].push_back(u);  // Grafo no dirigido
    }

    // Método para mostrar el grafo (ignorando nodos eliminados)
    void mostrar() {
        for (int i = 0; i < numNodos; ++i) {
            if (!verticesEliminados[i]) {
                cout << "Nodo " << i << ": ";
                for (int j : adyacencia[i]) {
                    if (!verticesEliminados[j]) {  // Ignorar aristas a nodos eliminados
                        cout << j << " ";
                    }
                }
                cout << endl;
            }
        }
    }

    // Método para percolar aristas con probabilidad p
    void percolarAristas(double p) {
        srand(time(0));
        for (int i = 0; i < numNodos; ++i) {
            if (verticesEliminados[i]) continue;  // Ignorar nodos eliminados
            for (int j : adyacencia[i]) {
                if (i < j && !verticesEliminados[j] && (rand() / double(RAND_MAX)) < p) {  // Eliminar arista con probabilidad p
                    cout << "Arista eliminada: " << i << " - " << j << endl;
                    eliminarArista(i, j);
                }
            }
        }
    }

    // Método para percolar vértices con probabilidad p
    void percolarVertices(double p) {
        srand(time(0));
        for (int i = 0; i < numNodos; ++i) {
            if (!verticesEliminados[i] && (rand() / double(RAND_MAX)) < p) {  // Eliminar vértice con probabilidad p
                cout << "Vértice eliminado: " << i << endl;
                eliminarVertice(i);
            }
        }
    }

    // Método para reindexar los vértices restantes y actualizar las aristas
    void reindexarVertices() {
        unordered_map<int, int> mapaReindexacion;
        int nuevoIndice = 0;

        // Crear el mapa de reindexación, asignando nuevos índices consecutivos a los nodos no eliminados
        for (int i = 0; i < numNodos; ++i) {
            if (!verticesEliminados[i]) {
                mapaReindexacion[i] = nuevoIndice++;
            }
        }

        // Crear una nueva lista de adyacencia con los vértices reindexados
        vector<vector<int>> nuevaAdyacencia(nuevoIndice);

        for (int i = 0; i < numNodos; ++i) {
            if (!verticesEliminados[i]) {
                for (int j : adyacencia[i]) {
                    if (!verticesEliminados[j]) {
                        int nuevoI = mapaReindexacion[i];
                        int nuevoJ = mapaReindexacion[j];
                        nuevaAdyacencia[nuevoI].push_back(nuevoJ);
                    }
                }
            }
        }

        // Actualizar el número de nodos
        numNodos = nuevoIndice;
        adyacencia = nuevaAdyacencia;
    }

    // Método para guardar el grafo en un archivo (reindexado)
    void guardarGrafo(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "No se pudo abrir el archivo para guardar el grafo: " << nombreArchivo << endl;
            return;
        }

        archivo << numNodos << endl;  // Guardar el número de nodos

        for (int i = 0; i < numNodos; ++i) {
            for (int j : adyacencia[i]) {
                if (i < j) {  // Guardar solo una vez cada arista
                    archivo << i << " " << j << endl;
                }
            }
        }

        archivo.close();
        cout << "Grafo guardado en " << nombreArchivo << endl;
    }

private:
    // Método para eliminar una arista
    void eliminarArista(int u, int v) {
        adyacencia[u].erase(std::remove(adyacencia[u].begin(), adyacencia[u].end(), v), adyacencia[u].end());
        adyacencia[v].erase(std::remove(adyacencia[v].begin(), adyacencia[v].end(), u), adyacencia[v].end());
    }

    // Método para marcar un vértice como eliminado
    void eliminarVertice(int v) {
        verticesEliminados[v] = true;
        for (int u : adyacencia[v]) {
            eliminarArista(v, u);
        }
    }
};

// Función para cargar el grafo desde un archivo
Grafo cargarGrafo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        exit(1);
    }

    int numNodos;
    archivo >> numNodos;
    Grafo grafo(numNodos);

    int u, v;
    while (archivo >> u >> v) {
        grafo.agregarArista(u, v);
    }

    archivo.close();
    return grafo;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <grafo.txt> <q> [a/v]" << endl;
        return 1;
    }

    string nombreArchivo = argv[1];
    double q = atof(argv[2]);  // Convertir q a un valor decimal
    double p = 1.0 - q;  // Probabilidad de eliminar un vértice o arista
    string modo = (argc > 3) ? argv[3] : "";

    // Cargar el grafo
    Grafo grafo = cargarGrafo(nombreArchivo);

    // Mostrar el grafo original
    cout << "Grafo original:" << endl;
    grafo.mostrar();

    // Realizar la percolación
    if (modo == "a") {
        cout << "Percolación de aristas:" << endl;
        grafo.percolarAristas(p);
    } else if (modo == "v") {
        cout << "Percolación de vértices:" << endl;
        grafo.percolarVertices(p);
    } else {
        cout << "Percolación completa (aristas y vértices):" << endl;
        grafo.percolarAristas(p);
        grafo.percolarVertices(p);
    }

    // Reindexar los vértices después de la percolación
    grafo.reindexarVertices();

    // Mostrar el grafo después de la percolación y reindexación
    cout << "Grafo después de la percolación y reindexación:" << endl;
    grafo.mostrar();

    // Guardar el grafo percolado y reindexado en un archivo
    grafo.guardarGrafo(argv[4]);

    return 0;
}
