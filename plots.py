import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def leer_datos(archivo_original, archivo_percolado):
    # Leer los datos del archivo de componentes originales
    datos_originales = pd.read_csv(archivo_original, sep=":", header=None, names=['Descripcion', 'Componentes_Originales'])
    datos_originales['n'] = datos_originales['Descripcion'].str.extract(r'n=(\d+)').astype(int)
    datos_originales['Muestra'] = datos_originales['Descripcion'].str.extract(r'Muestra (\d+)').astype(int)
    
    # Leer los datos del archivo de componentes percolados
    datos_percolados = pd.read_csv(archivo_percolado, sep=":", header=None, names=['Descripcion', 'Componentes_Percoladas'])
    datos_percolados['n'] = datos_percolados['Descripcion'].str.extract(r'n=(\d+)').astype(int)
    datos_percolados['Muestra'] = datos_percolados['Descripcion'].str.extract(r'Muestra (\d+)').astype(int)
    datos_percolados['q'] = datos_percolados['Descripcion'].str.extract(r'q=(0\.\d+)').astype(float)
    
    return datos_originales, datos_percolados

def plot_componentes_vs_q(datos_percolados):
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=datos_percolados, x='q', y='Componentes_Percoladas', hue='n', marker='o')
    plt.title('Fracción de Componentes Conexas vs Probabilidad de Percolación')
    plt.xlabel('Probabilidad de Percolación (q)')
    plt.ylabel('Número de Componentes Conexas')
    plt.legend(title='Tamaño de Grafo (n)', bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('componentes_vs_q.png')
    plt.close()

def plot_probabilidad_conectividad_completa(datos_percolados):
    # Agrupamos por q y calculamos la media de conectividad completa
    prob_conectividad = datos_percolados.groupby('q')['Componentes_Percoladas'].apply(lambda x: (x == 1).mean()).reset_index()
    
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=prob_conectividad, x='q', y='Componentes_Percoladas', marker='o')
    plt.title('Probabilidad de Conectividad Completa vs Probabilidad de Percolación')
    plt.xlabel('Probabilidad de Percolación (q)')
    plt.ylabel('Probabilidad de Conectividad Completa')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('probabilidad_conectividad_completa.png')
    plt.close()

def plot_boxplot_componentes(datos_percolados):
    plt.figure(figsize=(12, 8))
    sns.boxplot(data=datos_percolados, x='n', y='Componentes_Percoladas', hue='n', palette='Set2', dodge=False)
    plt.title('Variabilidad del Número de Componentes Conexas para Diferentes Tamaños de Grafo')
    plt.xlabel('Tamaño del Grafo (n)')
    plt.ylabel('Número de Componentes Conexas')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('boxplot_componentes.png')
    plt.close()

if __name__ == "__main__":
    archivo_original = 'DATASET_GEOMETRICO/componentes_originales.txt'
    archivo_percolado = 'DATASET_GEOMETRICO/componentes_percolados.txt'
    
    # Leer los datos desde los archivos de salida generados por el experimento
    datos_originales, datos_percolados = leer_datos(archivo_original, archivo_percolado)
    
    # Generar las visualizaciones y guardarlas como archivos de imagen
    plot_componentes_vs_q(datos_percolados)
    plot_probabilidad_conectividad_completa(datos_percolados)
    plot_boxplot_componentes(datos_percolados)
