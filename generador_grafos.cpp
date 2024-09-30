#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

// Función para generar un grafo aleatorio
void generarGrafoAleatorio(int numNodos, double densidad, const string& archivoSalida) {
    ofstream archivo(archivoSalida);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de salida: " << archivoSalida << endl;
        exit(1);
    }

    archivo << numNodos << endl;

    srand(time(0));

    int maxAristas = numNodos * (numNodos - 1) / 2;
    int numAristas = static_cast<int>(maxAristas * densidad);

    vector<pair<int, int>> aristasGeneradas;

    while (aristasGeneradas.size() < numAristas) {
        int u = rand() % numNodos;
        int v = rand() % numNodos;

        if (u != v) {
            if (u > v) swap(u, v);

            bool existe = false;
            for (const auto& arista : aristasGeneradas) {
                if (arista.first == u && arista.second == v) {
                    existe = true;
                    break;
                }
            }

            if (!existe) {
                aristasGeneradas.emplace_back(u, v);
                archivo << u << " " << v << endl;
            }
        }
    }

    archivo.close();
    cout << "Grafo aleatorio generado y guardado en " << archivoSalida << endl;
}

// Función para generar un grafo cuadrado
void generarGrafoCuadrado(int n, const string& archivoSalida) {
    ofstream archivo(archivoSalida);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de salida: " << archivoSalida << endl;
        exit(1);
    }

    int numNodos = n * n;
    archivo << numNodos << endl;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int nodoActual = i * n + j;
            if (j < n - 1) {
                archivo << nodoActual << " " << nodoActual + 1 << endl;
            }
            if (i < n - 1) {
                archivo << nodoActual << " " << nodoActual + n << endl;
            }
        }
    }

    archivo.close();
    cout << "Grafo cuadrado generado y guardado en " << archivoSalida << endl;
}

// Función para generar un grafo geométrico basado en un radio
void generarGrafoGeometrico(double radio, const string& archivoSalida) {
    ofstream archivo(archivoSalida);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de salida: " << archivoSalida << endl;
        exit(1);
    }

    int numNodos = rand() % 100 + 1;
    archivo << numNodos << endl;

    vector<pair<double, double>> puntos;

    srand(time(0));
    for (int i = 0; i < numNodos; ++i) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        puntos.push_back({x, y});
    }

    for (int i = 0; i < numNodos; ++i) {
        for (int j = i + 1; j < numNodos; ++j) {
            double distancia = sqrt(pow(puntos[i].first - puntos[j].first, 2) +
                                    pow(puntos[i].second - puntos[j].second, 2));
            if (distancia <= radio) {
                archivo << i << " " << j << endl;
            }
        }
    }

    archivo.close();
    cout << "Grafo geométrico generado con radio " << radio << " y guardado en " << archivoSalida << endl;
}

// Función para generar un grafo triangular
void generarGrafoTriangular(int n, const string& archivoSalida) {
    ofstream archivo(archivoSalida);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de salida: " << archivoSalida << endl;
        exit(1);
    }

    int numNodos = n * (n + 1) / 2;
    archivo << numNodos << endl;

    int contador = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (j < i - 1) {
                archivo << contador << " " << contador + 1 << endl;
            }
            if (i < n) {
                archivo << contador << " " << contador + i << endl;
            }
            ++contador;
        }
    }

    archivo.close();
    cout << "Grafo triangular generado y guardado en " << archivoSalida << endl;
}

int main(int argc, char* argv[]) {
    srand(time(0));

    int numNodos;
    double densidad = ((double)rand() / (RAND_MAX));
    string archivoSalida = "grafo.txt";

    if (argc == 2) {
        // Caso 1: ./generador_grafo archivo_salida.txt (n y densidad aleatorios)
        archivoSalida = argv[1];
        numNodos = rand() % 100 + 1;
        generarGrafoAleatorio(numNodos, densidad, archivoSalida);
    }
    else if (argc == 3) {
        if (argv[1][0] == '0') {
            // Caso 2: ./generador_grafo 0 d archivo_salida.txt (n aleatorio, densidad especificada)
            densidad = atof(argv[2]);
            numNodos = rand() % 100 + 1;
            generarGrafoAleatorio(numNodos, densidad, archivoSalida); 
            }
    }

    else if (argc == 4) {
        archivoSalida = argv[3];
        if (argv[2][0] == 'q') {
            // Caso 3: ./generador_grafo n "q" archivo_salida.txt (grafo cuadrado)
            numNodos = atoi(argv[1]);
            generarGrafoCuadrado(numNodos, archivoSalida);
        } else if (argv[2][0] == 'r') {
            // Caso 4: ./generador_grafo n "r" archivo_salida.txt (grafo geométrico)
            double radio = atof(argv[1]);
            generarGrafoGeometrico(radio, archivoSalida);
        } else if (argv[2][0] == 't') {
            // Caso 5: ./generador_grafo n "t" archivo_salida.txt (grafo triangular)
            numNodos = atoi(argv[1]);
            generarGrafoTriangular(numNodos, archivoSalida);
        }
    } else {
        cerr << "Uso: ./generador_grafos [n] [opcion] archivo_salida.txt" << endl;
        return 1;
    }

    return 0;
}
