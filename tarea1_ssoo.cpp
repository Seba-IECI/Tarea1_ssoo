#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <random>

using namespace std;

// Variables globales para la carrera
int distanciaTotal;        // Distancia total de la carrera
int numeroAutos;           // Número total de autos
mutex mtx;                 // Mutex para sincronizar la salida a la terminal
vector<pair<int, string>> resultados;  // Almacena el orden de llegada

// Función para generar un número aleatorio en un rango
int generarAleatorio(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// Función que simula la carrera de un auto
void correrAuto(int id) {
    int distanciaRecorrida = 0;
    int lugarLlegada = 0;
    string nombreAuto = "Auto" + to_string(id);

    while (distanciaRecorrida < distanciaTotal) {
        // Generar una distancia aleatoria entre 1 y 10 metros
        int distanciaAvance = generarAleatorio(1, 10);
        distanciaRecorrida += distanciaAvance;

        // Asegurarse de que no exceda la distancia total
        if (distanciaRecorrida > distanciaTotal) {
            distanciaRecorrida = distanciaTotal;
        }

        // Mostrar el avance del auto
        {
            lock_guard<mutex> lock(mtx);
            cout << nombreAuto << " avanza " << distanciaAvance 
                 << " metros (total: " << distanciaRecorrida << " metros)" << endl;
        }

        // Detenerse por un tiempo aleatorio entre 100 y 500 ms (simula la velocidad)
        this_thread::sleep_for(chrono::milliseconds(generarAleatorio(100, 500)));
    }

    // Al terminar la carrera, registrar el lugar de llegada
    {
        lock_guard<mutex> lock(mtx);
        lugarLlegada = resultados.size() + 1;
        resultados.push_back({lugarLlegada, nombreAuto});
        cout << nombreAuto << " termina la carrera en el lugar " << lugarLlegada << "!" << endl;
    }
}

int main(int argc, char* argv[]) {
    // Comprobar si se han pasado los parámetros correctamente
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <distancia_total> <numero_autos>" << endl;
        return 1;
    }

    // Leer los parámetros de la carrera
    distanciaTotal = stoi(argv[1]);
    numeroAutos = stoi(argv[2]);

    // Mostrar la información inicial de la carrera
    cout << "Distancia total de la carrera: " << distanciaTotal << " metros" << endl;
    cout << "Número de autos: " << numeroAutos << endl;
    cout << "----------------------------" << endl;

    // Crear hebras para cada auto
    vector<thread> hebras;
    for (int i = 0; i < numeroAutos; ++i) {
        hebras.push_back(thread(correrAuto, i));
    }

    // Esperar a que todas las hebras terminen
    for (auto& t : hebras) {
        t.join();
    }

    // Mostrar el resultado final
    cout << "----------------------------" << endl;
    cout << "Resultados finales de la carrera:" << endl;
    cout << "Lugar\tAuto" << endl;
    for (const auto& resultado : resultados) {
        cout << resultado.first << "\t" << resultado.second << endl;
    }

    return 0;
}
