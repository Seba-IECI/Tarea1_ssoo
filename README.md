# Tarea 1 de sistemas operativos
Instrucciones de ejecucion:
1. Abrir compilador GNU
2. nano
3. pegar codigo
4. guardar con nombre: carrera.cpp
5. nano
6. pegar el siguiente codigo:
   
CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: carrera

carrera: carrera.o
	$(CXX) $(CXXFLAGS) -o carrera carrera.o

carrera.o: carrera.cpp
	$(CXX) $(CXXFLAGS) -c carrera.cpp

clean:
	rm -f carrera carrera.o
 
 7. guardar con nombre: Makefile
 8. make
 9. ./carrera 100 3

