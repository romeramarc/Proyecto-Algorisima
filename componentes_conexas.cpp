#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// Función DFS para explorar el grafo
void dfs(int nodo, const vector<vector<int>> &grafo, vector<bool> &visitado) {
    visitado[nodo] = true;  // Marcar el nodo como visitado
    for (int vecino : grafo[nodo]) {
        if (!visitado[vecino]) {
            dfs(vecino, grafo, visitado);  // Explorar nodos adyacentes
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        return 1;
    }

    // Abrir el archivo de entrada
    ifstream archivo(argv[1]);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }

    // Leer el número de nodos del grafo
    int n;
    archivo >> n;

    // Inicializar la lista de adyacencia
    vector<vector<int>> grafo(n);

    // Leer las aristas del grafo desde el archivo
    int u, v;
    while (archivo >> u >> v) {
        grafo[u].push_back(v);
        grafo[v].push_back(u);  // Debido a que el grafo es no dirigido
    }

    archivo.close();

    // Vector para marcar los nodos visitados
    vector<bool> visitado(n, false);

    // Contador de componentes conexas
    int componentesConexas = 0;

    // Recorrer todos los nodos para encontrar las componentes conexas
    for (int i = 0; i < n; ++i) {
        if (!visitado[i]) {
            // Si el nodo no ha sido visitado, lanzamos una DFS
            dfs(i, grafo, visitado);
            componentesConexas++;  // Nueva componente encontrada
        }
    }

    // Mostrar el número de componentes conexas
    cout << "Número de componentes conexas: " << componentesConexas << endl;

    return 0;
}
