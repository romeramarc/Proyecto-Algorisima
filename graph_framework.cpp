#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <random>
#include <string>
#include <algorithm>

using namespace std;

class Grafo {
public:
    int numNodos;
    vector<vector<int>> adyacencia;
    vector<bool> verticesEliminados;

    // Constructor para inicializar el grafo
    Grafo(int n) : numNodos(n), adyacencia(n), verticesEliminados(n, false) {}

    // Método para agregar una arista al grafo
    void agregarArista(int u, int v) {
        adyacencia[u].push_back(v);
        adyacencia[v].push_back(u);
    }

    // Método para generar un grafo cuadrado de tamaño nxn
    static Grafo generarGrafoCuadrado(int n) {
        int numNodos = n * n;
        Grafo grafo(numNodos);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int nodoActual = i * n + j;
                if (j < n - 1) grafo.agregarArista(nodoActual, nodoActual + 1);   // Conectar con el nodo a la derecha
                if (i < n - 1) grafo.agregarArista(nodoActual, nodoActual + n);   // Conectar con el nodo de abajo
            }
        }
        return grafo;
    }

    // Método para generar un grafo triangular basado en niveles
    static Grafo generarGrafoTriangular(int niveles) {
        int numNodos = niveles * (niveles + 1) / 2;
        Grafo grafo(numNodos);
        int contador = 0;
        for (int i = 1; i <= niveles; ++i) {
            for (int j = 0; j < i; ++j) {
                if (j < i - 1) grafo.agregarArista(contador, contador + 1);  // Conectar con el nodo a la derecha
                if (i < niveles) grafo.agregarArista(contador, contador + i);  // Conectar con el nodo de abajo
                ++contador;
            }
        }
        return grafo;
    }

    // Método para generar un grafo geométrico con radio crítico más 5
    static Grafo generarGrafoGeometrico(int n) {
        double radio = sqrt(log(n) / (M_PI * n)) + 5;  // Radio crítico más ajuste para garantizar la conexión
        Grafo grafo(n);
        vector<pair<double, double>> puntos;
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 10);

        // Generar puntos aleatorios en el plano
        for (int i = 0; i < n; ++i) {
            puntos.emplace_back(dis(gen), dis(gen));
        }

        // Conectar nodos si están a una distancia menor o igual al radio crítico ajustado
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double distancia = sqrt(pow(puntos[i].first - puntos[j].first, 2) + pow(puntos[i].second - puntos[j].second, 2));
                if (distancia <= radio) {
                    grafo.agregarArista(i, j);
                }
            }
        }
        return grafo;
    }

    // Método para percolar aristas con probabilidad p = 1 - q
    void percolarAristas(double p) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dist(0.0, 1.0);

        for (int i = 0; i < numNodos; ++i) {
            if (verticesEliminados[i]) continue;  // Ignorar nodos ya eliminados
            for (auto it = adyacencia[i].begin(); it != adyacencia[i].end();) {
                int j = *it;
                if (dist(gen) < p) {  // Eliminar la arista con probabilidad p
                    it = adyacencia[i].erase(it);  // Eliminar arista y actualizar el iterador
                    adyacencia[j].erase(remove(adyacencia[j].begin(), adyacencia[j].end(), i), adyacencia[j].end());
                } else {
                    ++it;
                }
            }
        }
    }

    // Método para percolar vértices con probabilidad p = 1 - q
    void percolarVertices(double p) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dist(0.0, 1.0);

        for (int i = 0; i < numNodos; ++i) {
            if (!verticesEliminados[i] && dist(gen) < p) {
                verticesEliminados[i] = true;  // Marcar vértice como eliminado
            }
        }
    }

    // Método para calcular el número de componentes conexas
    int calcularComponentesConexas() const {
        vector<bool> visitado(numNodos, false);
        int componentesConexas = 0;

        for (int i = 0; i < numNodos; ++i) {
            if (!visitado[i] && !verticesEliminados[i]) {
                dfs(i, visitado);
                componentesConexas++;
            }
        }

        return componentesConexas;
    }

private:
    // Método para realizar una búsqueda en profundidad (DFS)
    void dfs(int nodo, vector<bool> &visitado) const {
        visitado[nodo] = true;
        for (int vecino : adyacencia[nodo]) {
            if (!visitado[vecino] && !verticesEliminados[vecino]) {
                dfs(vecino, visitado);
            }
        }
    }

    // Método para eliminar una arista
    void eliminarArista(int u, int v) {
        adyacencia[u].erase(remove(adyacencia[u].begin(), adyacencia[u].end(), v), adyacencia[u].end());
        adyacencia[v].erase(remove(adyacencia[v].begin(), adyacencia[v].end(), u), adyacencia[v].end());
    }
};

// Función para realizar el experimento con los parámetros configurados
void realizarExperimentoConArchivos(int numGrafoMin, int numGrafoMax, int stepN, int grafosPorN, const string& tipoGrafo, const string& tipoPercolacion, const string& archivoOriginal, const string& archivoPercolado) {
    ofstream archivoCompOriginal(archivoOriginal);
    ofstream archivoCompPercolado(archivoPercolado);

    if (!archivoCompOriginal.is_open() || !archivoCompPercolado.is_open()) {
        cerr << "Error al abrir los archivos de salida." << endl;
        return;
    }

    for (int n = numGrafoMin; n <= numGrafoMax; n += stepN) {
        for (int i = 0; i < grafosPorN; ++i) {
            Grafo grafo = (tipoGrafo == "cuadrado") ? Grafo::generarGrafoCuadrado(n) :
                          (tipoGrafo == "triangular") ? Grafo::generarGrafoTriangular(n) :
                          Grafo::generarGrafoGeometrico(n);

            int componentesOriginales = grafo.calcularComponentesConexas();
            archivoCompOriginal << "Grafo n=" << n << " - Muestra " << i + 1 << ": " << componentesOriginales << endl;

            for (double q = 0.1; q <= 0.9; q += 0.1) {
                double p = 1.0 - q;  // Probabilidad de eliminar aristas o vértices
                Grafo grafoPercolado = grafo;  // Copia profunda del grafo original para cada valor de q
                
                if (tipoPercolacion == "aristas") {
                    grafoPercolado.percolarAristas(p);
                } else if (tipoPercolacion == "vertices") {
                    grafoPercolado.percolarVertices(p);
                } else if (tipoPercolacion == "ambos") {
                    grafoPercolado.percolarAristas(p);
                    grafoPercolado.percolarVertices(p);
                }

                int componentesPercoladas = grafoPercolado.calcularComponentesConexas();
                archivoCompPercolado << "Grafo n=" << n << " - Muestra " << i + 1 << " - q=" << q << " - p=" << p << ": " << componentesPercoladas << endl;
            }
        }
    }

    archivoCompOriginal.close();
    archivoCompPercolado.close();
}

int main() {
    // Parámetros del experimento
    int numGrafoMin = 10;    // Valor mínimo de n para los grafos
    int numGrafoMax = 100;   // Valor máximo de n para los grafos
    int stepN = 10;          // Incremento del tamaño de n en cada iteración
    int grafosPorN = 20;      // Número de grafos a generar por cada valor de n
    string tipoGrafo = "triangular"; // Tipo de grafo: "cuadrado", "triangular", o "geometrico"
    string tipoPercolacion = "aristas"; // Tipo de percolación: "aristas", "vertices", o "ambos"
    string archivoOriginal = "componentes_originales.txt";
    string archivoPercolado = "componentes_percolados.txt";

    // Ejecutar el experimento con los parámetros configurados
    realizarExperimentoConArchivos(numGrafoMin, numGrafoMax, stepN, grafosPorN, tipoGrafo, tipoPercolacion, archivoOriginal, archivoPercolado);

    return 0;
}
