Marco Estratégico General para la Migración de Procesos SAS a Databricks

1. Objetivo del Marco Estratégico

El objetivo de este marco estratégico es definir los lineamientos clave que permitan la migración eficiente de los procesos SAS al entorno Databricks. Este marco se enfoca en la reutilización de componentes, la modularización de la lógica, la centralización de parámetros, la automatización de procesos y el aprovechamiento de las capacidades de Databricks para el procesamiento distribuido de grandes volúmenes de datos.

Este documento se ha construido a partir del análisis de los procesos previamente estudiados y se ha diseñado para ser aplicable a otros procesos similares, asegurando una migración eficiente, escalable y fácil de mantener.

2. Estructura del Marco Estratégico

El marco se basa en los siguientes puntos clave:
	•	Centralizar configuraciones y parámetros
	•	Modularizar la lógica de negocio
	•	Implementar uniones y combinaciones de datos estandarizadas
	•	Automatizar la generación de períodos y fechas
	•	Estandarizar operaciones de agregación y resumen
	•	Almacenamiento y persistencia de datos

3. Descripción y acciones clave por cada punto

1. Centralizar configuraciones y parámetros

Descripción:
Los procesos SAS suelen tener configuraciones distribuidas en forma de macros (%let) o variables globales. Este enfoque puede complicar la mantenibilidad y la escalabilidad del proceso. En Databricks, se propone centralizar estas configuraciones en archivos YAML o JSON que se carguen de forma dinámica al inicio de la ejecución.

Acciones recomendadas:
	•	Crear archivos YAML o JSON con los parámetros de configuración.
	•	Cargar y leer la configuración al inicio de la ejecución del proceso en Databricks.
	•	Evitar la codificación de valores fijos en el código.

Ejemplo de archivo de configuración YAML (config.yaml):

filtros:
  ramo: ['FC', 'FD', 'FK']
  periodo_vto: 202410
  max_maduracion: 3
  num_periodos: 6

Ejemplo de código en PySpark para cargar la configuración:

import yaml

with open("/dbfs/mnt/config/config.yaml", "r") as file:
    config = yaml.safe_load(file)

filtros_ramo = config["filtros"]["ramo"]
periodo_vto = config["filtros"]["periodo_vto"]
max_maduracion = config["filtros"]["max_maduracion"]

2. Modularizar la lógica de negocio

Descripción:
Las operaciones comunes deben encapsularse en funciones reutilizables para evitar la duplicación de lógica en el código. Esto incluye funciones de filtrado, cálculos de fechas, generación de columnas, entre otros.

Acciones recomendadas:
	•	Identificar funciones repetitivas en los procesos SAS.
	•	Crear módulos Python con funciones reutilizables para la lógica de negocio.
	•	Importar las funciones desde un módulo centralizado.

Ejemplo de funciones reutilizables:

from pyspark.sql import functions as F

def filtrar_por_ramo(df, ramos):
    """Filtra el DataFrame por la lista de ramos especificada."""
    return df.filter(F.col("ramo").isin(ramos))

def calcular_maduracion(df, anio_col, mes_col, max_maduracion):
    """Calcula los períodos de maduración inicial y final."""
    df = df.withColumn("aa_ini", F.when(F.col(mes_col) - 2 <= 0, F.col(anio_col) - 1).otherwise(F.col(anio_col)))
    df = df.withColumn("mm_ini", F.when(F.col(mes_col) - 2 <= 0, 12 + F.col(mes_col) - 2).otherwise(F.col(mes_col) - 2))
    return df

3. Implementar uniones y combinaciones de datos estandarizadas

Descripción:
Los procesos SAS realizan uniones de tablas de forma frecuente. Para estandarizar esta operación, se propone utilizar una función genérica para realizar uniones con control sobre el tipo de unión y las claves de relación.

Acciones recomendadas:
	•	Crear una función para uniones genéricas.
	•	Definir parámetros para las claves de unión y el tipo de unión.

Ejemplo de función de unión:

def unir_tablas(df1, df2, join_cols, join_type="left"):
    """
    Realiza la unión de dos DataFrames con el tipo de unión especificado.
    """
    return df1.join(df2, on=join_cols, how=join_type)

4. Automatizar la generación de períodos y fechas

Descripción:
En SAS, los períodos se calculan mediante macros y manipulación de cadenas. En Databricks, se pueden calcular automáticamente utilizando funciones de datetime y timedelta o con las funciones nativas de PySpark.

Acciones recomendadas:
	•	Crear una función para la generación automática de períodos.
	•	Utilizar datetime y timedelta para calcular fechas dinámicamente.

Ejemplo de función de generación de períodos:

from datetime import datetime, timedelta

def generar_periodos(periodo_inicial, num_periodos):
    periodos = []
    fecha_inicial = datetime.strptime(str(periodo_inicial), "%Y%m")
    for i in range(num_periodos):
        periodo = (fecha_inicial - timedelta(days=30 * i)).strftime("%Y%m")
        periodos.append(int(periodo))
    return periodos

5. Estandarizar operaciones de agregación y resumen

Descripción:
Estandarizar la lógica de los cálculos de agregación (sumas, promedios, conteos) para evitar duplicación de lógica y mantener la consistencia de los resultados.

Acciones recomendadas:
	•	Crear una función reutilizable para realizar operaciones de agregación de forma genérica.
	•	Definir parámetros para las columnas de agrupación y los cálculos de agregación.

Ejemplo de función de agregación de datos:

from pyspark.sql import functions as F

def agregar_datos(df, group_by_cols, agg_exprs):
    """
    Realiza una agregación sobre un DataFrame, agrupando por las columnas especificadas y aplicando las expresiones de agregación.
    """
    return df.groupBy(group_by_cols).agg(*[F.expr(expr) for expr in agg_exprs])

6. Almacenamiento y persistencia de datos

Descripción:
Asegurar que la tabla final se almacene en un formato optimizado (como Parquet o Delta), permitiendo su reutilización en otros procesos.

Acciones recomendadas:
	•	Definir una ruta de almacenamiento centralizada en el Data Lake.
	•	Usar Delta Lake para permitir actualizaciones incrementales y mejor control de la tabla final.

Ejemplo de persistencia de la tabla final:

output_path = "/mnt/datalake/tables/final_table"

# Usar Delta para permitir actualizaciones incrementales
df.write.format("delta").mode("overwrite").save(output_path)

4. Conclusiones finales

El marco estratégico descrito proporciona un enfoque integral para la migración de procesos SAS a Databricks, asegurando la modularización, la reutilización y la escalabilidad. Este enfoque permite optimizar la lógica de negocio, centralizar parámetros y facilitar la actualización de los procesos en el futuro. Además, el uso de Delta Lake garantiza la persistencia eficiente y la capacidad de realizar actualizaciones incrementales.

Este marco es aplicable a procesos similares y puede evolucionar conforme se identifiquen nuevos patrones o necesidades en la migración de los procesos SAS a Databricks.

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

### Compilación del Código
Para compilar el código en la terminal, navega hasta el directorio donde se encuentra el archivo del proyecto y ejecuta el siguiente comando:

```bash
make
```
### Ejecución del Código

```bash
./graph_framework
```

### Modificar parametros para personalizar las muestras

Estos son los valores que encontramos en el main y se pueden modifcar manualmente.

```cpp
int numGrafoMin = 50;
int numGrafoMax = 500;
int stepN = 50;
string tipoGrafo = "cuadrado";
string tipoPercolacion = "vertices";
```

## Instrucciones para Crear los Graficos y analizar las muestras

En caso de querer los plots para visualizar los datos de una muestra en particular:

1. Especificamos la ruta de los archivos
```py
if __name__ == "__main__":
    archivo_original = 'DATASET_GEOMETRICO/componentes_originales.txt'
    archivo_percolado = 'DATASET_GEOMETRICO/componentes_percolados.txt'
```

2. Ejecutamos
```bash
python3 plot.py
```
En caso de querer los plots para comprar los grafos:

```bash
python3 plot_completo.py
```


