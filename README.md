# Proyecto de Algorismia: Conectividad y Percolación

Este proyecto forma parte del curso de Algorismia en la **FIB-UPC** (Facultat d'Informàtica de Barcelona, Universitat Politècnica de Catalunya) durante el trimestre **Q1 2024-2025**. Su objetivo principal es realizar un estudio experimental sobre la existencia de una posible transición de fase en el número de componentes conexas de un grafo cuando se somete a un proceso de percolación.

## Objetivos del Proyecto

### Objetivo Principal
El proyecto se enfoca en analizar cómo cambia la estructura de un grafo al someterse a un proceso de percolación, tanto en nodos como en aristas, y estudiar la posible existencia de una **transición de fase** para ciertas propiedades de los grafos.

### Objetivos Secundarios
1. **Estudio de grafos aleatorios y deterministas:** Experimentar con diferentes modelos de grafos para entender su comportamiento bajo percolación.
2. **Análisis de propiedades de grafos:** Observar cómo cambian propiedades como la conectividad al variar la probabilidad de fallo en nodos y aristas.
3. **Implementación de algoritmos eficientes:** Implementar en C++ algoritmos para generar grafos, aplicar procesos de percolación y calcular componentes conexas.

## Descripción del Proyecto

### Tipos de Grafos Analizados
El proyecto utiliza diferentes modelos de grafos para el análisis:
- **Grafo Cuadrado:** Una cuadrícula de tamaño `n x n` donde cada nodo se conecta a su vecino derecho y al de abajo.
- **Grafo Triangular:** Un grafo con niveles, donde el número de nodos aumenta progresivamente en cada nivel.
- **Grafo Geométrico Aleatorio:** Grafos donde los nodos se conectan si la distancia entre ellos es menor que un cierto radio.

### Procesos de Percolación
Se emplean dos tipos de percolación en este proyecto:
1. **Percolación de Nodos (Site Percolation):** Cada nodo tiene una probabilidad `q` de permanecer en el grafo o ser eliminado con una probabilidad `1 - q`.
2. **Percolación de Aristas (Bond Percolation):** Cada arista tiene una probabilidad `q` de ser eliminada.

El proceso de percolación se realiza de la siguiente manera:
- En la **percolación de nodos**, cada nodo se mantiene con una probabilidad `q` y se elimina con una probabilidad `1 - q`. Esto significa que algunos nodos pueden desaparecer completamente, junto con sus conexiones.
- En la **percolación de aristas**, cada conexión entre nodos se mantiene con una probabilidad `q` o se elimina con una probabilidad `1 - q`, afectando la estructura del grafo sin eliminar nodos directamente.

### Análisis de Transición de Fase
El objetivo es identificar valores de la probabilidad `q` en los que ocurre un cambio significativo en el número de componentes conexas del grafo, lo cual se conoce como **transición de fase**. Este análisis se realiza tanto para grafos deterministas como aleatorios.

Una transición de fase en este contexto significa que para ciertos valores de `q`, la estructura del grafo cambia abruptamente de tener muchas componentes desconectadas a volverse mayormente conexa.

## Instrucciones para Compilar y Ejecutar el Código

### Requisitos Previos
- Tener un compilador de C++ instalado (como `g++`).
- Terminal o línea de comandos.

### Compilación del Código
Para compilar el código en la terminal, navega hasta el directorio donde se encuentra el archivo del proyecto y ejecuta el siguiente comando:

```bash
g++ -o percolacion main.cpp
