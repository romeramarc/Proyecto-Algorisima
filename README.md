# Importar librerías necesarias
from pyspark.sql import SparkSession
from pyspark.sql.types import StructType, StructField, StringType, IntegerType, DateType, DecimalType
from pyspark.sql.functions import col, when, regexp_extract

# 1. Crear la sesión de Spark
spark = SparkSession.builder \
    .appName("Comparar múltiples esquemas con tablas reales") \
    .getOrCreate()

# 2. Ruta al archivo Excel (cambia la ruta según tu archivo subido)
ruta_excel = '/FileStore/tables/archivo.xlsx'

# 3. Cargar el archivo Excel con la especificación del esquema
df_excel = spark.read \
    .format("com.crealytics.spark.excel") \
    .option("header", "true") \
    .option("inferSchema", "true") \
    .load(ruta_excel)

# 4. Filtrar las columnas necesarias
df_filtrado = df_excel.select("Nombre Tabla", "Variable", "Tipo", "Acción Fabric")

# 5. Crear esquemas para cada "Nombre Tabla"
from pyspark.sql.types import StructType, StructField

# Crear un diccionario para almacenar los esquemas generados para cada tabla
esquemas_por_tabla = {}

# Crear el esquema para cada tabla
for nombre_tabla in df_filtrado.select("Nombre Tabla").distinct().rdd.flatMap(lambda x: x).collect():
    df_tabla = df_filtrado.filter(col("Nombre Tabla") == nombre_tabla)
    
    # Aplicar las reglas de "Acción Fabric"
    df_tabla = df_tabla.withColumn(
        "nombre_final",
        when(col("Acción Fabric").rlike("(?i)cambiar nombre a"), 
             regexp_extract(col("Acción Fabric"), r'cambiar nombre a\s*"?(\w+)"?', 1)
        ).when(
            col("Acción Fabric").rlike("(?i)añadir nueva variable"), 
            col("Variable")
        ).otherwise(col("Variable"))
    )
    
    # Filtrar las variables a eliminar
    df_tabla = df_tabla.filter(df_tabla["Acción Fabric"] != "Eliminar")
    
    # Mapear los tipos de la tabla a tipos de PySpark
    tipo_mapeo = {
        'string': StringType(),
        'integer': IntegerType(),
        'date': DateType(),
        'Character': StringType(),
        'Numeric': IntegerType(),
        'decimal(10,7)': DecimalType(10, 7)
    }
    
    # Crear los campos del esquema
    campos = []
    for row in df_tabla.collect():
        nombre_campo = row['nombre_final']
        tipo_campo = row['Tipo'].strip().lower()
        tipo = tipo_mapeo.get(tipo_campo, StringType())  # Por defecto, se asume StringType
        campos.append(StructField(nombre_campo, tipo, True))
    
    # Crear y almacenar el esquema
    esquema_final = StructType(campos)
    esquemas_por_tabla[nombre_tabla] = esquema_final

# 6. Comparar los esquemas con las tablas reales
for nombre_tabla, esquema_generado in esquemas_por_tabla.items():
    print(f"\n🔍 Comparando la tabla: {nombre_tabla}")
    
    # Cargar la tabla real (se asume que ya tienes una función predefinida para cargar la tabla)
    try:
        df_tabla_real = spark.read.table(nombre_tabla)
    except Exception as e:
        print(f"❌ Error al cargar la tabla real: {nombre_tabla}. Detalle: {e}")
        continue
    
    # Extraer el esquema real de la tabla
    esquema_real = {campo.name: str(campo.dataType) for campo in df_tabla_real.schema.fields}
    
    # Extraer el esquema generado
    esquema_generado_dict = {campo.name: str(campo.dataType) for campo in esquema_generado.fields}
    
    # Comparar los nombres de columnas
    columnas_faltantes = set(esquema_generado_dict.keys()) - set(esquema_real.keys())
    columnas_sobrantes = set(esquema_real.keys()) - set(esquema_generado_dict.keys())
    columnas_comunes = set(esquema_generado_dict.keys()) & set(esquema_real.keys())
    
    # Verificar diferencias en los tipos de datos
    diferencias_tipos = {
        columna: {
            "tipo_generado": esquema_generado_dict[columna],
            "tipo_real": esquema_real[columna]
        }
        for columna in columnas_comunes
        if esquema_generado_dict[columna] != esquema_real[columna]
    }
    
    # Validar si la tabla es correcta
    if not columnas_faltantes and not columnas_sobrantes and not diferencias_tipos:
        print(f"✅ La tabla '{nombre_tabla}' está validada correctamente ✅")
    else:
        print(f"❌ Errores detectados en la tabla '{nombre_tabla}' ❌")
        
        if columnas_faltantes:
            print("\n❌ Columnas que deberían estar pero no se encuentran:")
            for columna in columnas_faltantes:
                print(f"  - {columna}")
        
        if columnas_sobrantes:
            print("\n❌ Columnas que están en la tabla pero no deberían estar:")
            for columna in columnas_sobrantes:
                print(f"  - {columna}")
        
        if diferencias_tipos:
            print("\n❌ Diferencias de tipo de datos entre el esquema generado y la tabla real:")
            for columna, tipos in diferencias_tipos.items():
                print(f"  - Columna: {columna}, Tipo generado: {tipos['tipo_generado']}, Tipo real: {tipos['tipo_real']}")


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


