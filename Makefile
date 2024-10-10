# Definir el compilador y las opciones para C++
CXX = g++

CXXFLAGS = -Wall -g

SRC1 = graph_framework.cpp

OBJ1 = graph_framework.o

EXE1 = graph_framework


# Compilación por defecto
all: $(EXE1)

# Reglas para crear cada ejecutable en C++
$(EXE1): $(OBJ1)
	$(CXX) $(CXXFLAGS) -o $(EXE1) $(OBJ1)

# Reglas para crear los archivos objeto en C++
$(OBJ1): $(SRC1)
	$(CXX) $(CXXFLAGS) -c $(SRC1)

# Limpiar archivos temporales
clean:
	rm -f *.o $(EXE1) 
