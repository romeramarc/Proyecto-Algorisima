import subprocess
import os

def ejecutar_programa(comando):
    """Ejecuta un programa de terminal y devuelve su salida."""
    resultado = subprocess.run(comando, stderr=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    
    if resultado.returncode != 0:
        print(f"Error al ejecutar {comando}: {resultado.stderr}")
        return None
    return resultado.stdout.strip()  # Devolver la salida limpia sin espacios extra

def extraer_numero_componentes(salida):
    """Extrae el número de componentes de la salida del programa componentes_conexas."""
    # Suponemos que la salida es del tipo "Número de componentes conexas: X"
    for linea in salida.splitlines():
        if "Número de componentes conexas:" in linea:
            # Extraemos solo el número después de la frase
            return linea.split(":")[1].strip()
    return None  # Si no encuentra la línea esperada, devuelve None

def generar_y_percolar_grafos(tipo_grafo, num_muestras, num_nodos, q, modo_percolacion):
    directorio_resultados = "resultados"
    
    # Asegurarse de que el directorio de resultados existe
    if not os.path.exists(directorio_resultados):
        os.makedirs(directorio_resultados)

    # Archivos para guardar las componentes conexas
    archivo_componentes_originales = os.path.join(directorio_resultados, "componentes_conexas_originales.txt")
    archivo_componentes_percolados = os.path.join(directorio_resultados, "componentes_conexas_percolados.txt")

    # Abrir archivos para almacenar las componentes conexas
    with open(archivo_componentes_originales, 'w') as comp_orig, open(archivo_componentes_percolados, 'w') as comp_perc:
        for i in range(1, num_muestras + 1):
            archivo_grafo = os.path.join(directorio_resultados, f"grafo_{tipo_grafo}_{num_nodos}_sample_{i}.txt")
            archivo_grafo_percolado = os.path.join(directorio_resultados, f"grafo_percolado_{tipo_grafo}_{num_nodos}_sample_{i}.txt")

            # Generar el grafo
            comando_generar = ["./generador_grafos", str(num_nodos), tipo_grafo[0], archivo_grafo]
            ejecutar_programa(comando_generar)

            # Aplicar la percolación y guardar el grafo percolado
            comando_percolar = ["./percolador_grafos", archivo_grafo, str(q), modo_percolacion, archivo_grafo_percolado]
            ejecutar_programa(comando_percolar)

            # Calcular las componentes conexas del grafo original
            comando_componentes_original = ["./componentes_conexas", archivo_grafo]
            salida_original = ejecutar_programa(comando_componentes_original)
            componentes_originales = extraer_numero_componentes(salida_original)
            if componentes_originales:
                comp_orig.write(f"{componentes_originales}\n")

            # Calcular las componentes conexas del grafo percolado
            comando_componentes_percolado = ["./componentes_conexas", archivo_grafo_percolado]
            salida_percolada = ejecutar_programa(comando_componentes_percolado)
            componentes_percoladas = extraer_numero_componentes(salida_percolada)
            if componentes_percoladas:
                comp_perc.write(f"{componentes_percoladas}\n")

# Solicitar los parámetros al usuario
num_muestras = int(input("Introduce el tamaño de la muestra: "))
num_nodos = int(input("Introduce el número de nodos por grafo: "))
tipo_grafo = input("Introduce el tipo de grafo (cuadrado (q) /geométrico (r) /triangular (t)): ")
q = float(input("Introduce el valor de q para la percolación (probabilidad de percolación): "))
modo_percolacion = input("Introduce el modo de percolación (a = aristas, v = vértices, av = ambos): ")

# Llamar a la función para generar y percolar grafos
generar_y_percolar_grafos(tipo_grafo, num_muestras, num_nodos, q, modo_percolacion)
