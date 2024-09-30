# Definir el compilador y las opciones para C++
CXX = g++
CXXFLAGS = -Wall -g

# Nombres de los archivos fuente y los ejecutables para C++
SRC1 = componentes_conexas.cpp
SRC2 = generador_grafos.cpp
SRC3 = percolador_grafos.cpp

OBJ1 = componentes_conexas.o
OBJ2 = generador_grafos.o
OBJ3 = percolador_grafos.o

# Nombres de los ejecutables para C++
EXE1 = componentes_conexas
EXE2 = generador_grafos
EXE3 = percolador_grafos

# Compilación por defecto
all: $(EXE1) $(EXE2) $(EXE3)

# Reglas para crear cada ejecutable en C++
$(EXE1): $(OBJ1)
	$(CXX) $(CXXFLAGS) -o $(EXE1) $(OBJ1)

$(EXE2): $(OBJ2)
	$(CXX) $(CXXFLAGS) -o $(EXE2) $(OBJ2)

$(EXE3): $(OBJ3)
	$(CXX) $(CXXFLAGS) -o $(EXE3) $(OBJ3)

# Reglas para crear los archivos objeto en C++
$(OBJ1): $(SRC1)
	$(CXX) $(CXXFLAGS) -c $(SRC1)

$(OBJ2): $(SRC2)
	$(CXX) $(CXXFLAGS) -c $(SRC2)

$(OBJ3): $(SRC3)
	$(CXX) $(CXXFLAGS) -c $(SRC3)

# Limpiar archivos temporales
clean:
	rm -f *.o $(EXE1) $(EXE2) $(EXE3)
