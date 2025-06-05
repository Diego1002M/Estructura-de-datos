#include <iostream>
#include <string>
using namespace std;

// Estructura de un proceso
struct Proceso {
    int id;
    string nombre;
    int prioridad;
    Proceso* siguiente;
    
    Proceso(int i, string n, int p) : id(i), nombre(n), prioridad(p), siguiente(NULL) {}
};

// Clase ListaEnlazada (Gestor de Procesos)
class ListaEnlazada {
private:
    Proceso* cabeza;
public:
    ListaEnlazada() : cabeza(NULL) {}
    
    void insertarProceso(int id, string nombre, int prioridad) {
        Proceso* nuevo = new Proceso(id, nombre, prioridad);
        if (cabeza == NULL || cabeza->id >= nuevo->id) {
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
        } else {
            Proceso* actual = cabeza;
            while (actual->siguiente != NULL && actual->siguiente->id < nuevo->id) {
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
    }
    
    void mostrarProcesos() {
        Proceso* actual = cabeza;
        while (actual != NULL) {
            cout << "ID: " << actual->id 
                 << ", Nombre: " << actual->nombre 
                 << ", Prioridad: " << actual->prioridad << endl;
            actual = actual->siguiente;
        }
    }
};

// Clase ColaPrioridad (Planificador de CPU)
class ColaPrioridad {
private:
    Proceso* frente;
public:
    ColaPrioridad() : frente(NULL) {}
    
    void encolar(Proceso* proceso) {
        if (frente == NULL || proceso->prioridad > frente->prioridad) {
            proceso->siguiente = frente;
            frente = proceso;
        } else {
            Proceso* actual = frente;
            while (actual->siguiente != NULL && actual->siguiente->prioridad >= proceso->prioridad) {
                actual = actual->siguiente;
            }
            proceso->siguiente = actual->siguiente;
            actual->siguiente = proceso;
        }
    }
    
    Proceso* desencolar() {
        if (frente == NULL) return NULL;
        Proceso* temp = frente;
        frente = frente->siguiente;
        return temp;
    }
};

// Clase PilaMemoria (Gestor de Memoria)
class PilaMemoria {
private:
    int* bloques;
    int tope;
    int capacidad;
public:
    PilaMemoria(int cap) : capacidad(cap), tope(-1) {
        bloques = new int[capacidad];
    }
    
    void push(int procesoId) {
        if (tope < capacidad - 1) {
            bloques[++tope] = procesoId;
        }
    }
    
    int pop() {
        if (tope >= 0) {
            return bloques[tope--];
        }
        return -1; // Valor inválido si la pila está vacía
    }
};

// Menú básico
void mostrarMenu() {
    cout << "\n=== Sistema de Gestión de Procesos ===" << endl;
    cout << "1. Añadir proceso" << endl;
    cout << "2. Mostrar procesos" << endl;
    cout << "3. Salir" << endl;
    cout << "Seleccione una opción: ";
}

int main() {
    ListaEnlazada lista;
    ColaPrioridad cola;
    
    int opcion, id, prioridad;
    string nombre;
    Proceso* nuevo = NULL; // Declarar fuera del switch
    
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                cout << "ID del proceso: ";
                cin >> id;
                cout << "Nombre del proceso: ";
                cin.ignore();
                getline(cin, nombre);
                cout << "Prioridad: ";
                cin >> prioridad;
                
                nuevo = new Proceso(id, nombre, prioridad); // Asignar aquí
                lista.insertarProceso(id, nombre, prioridad);
                cola.encolar(nuevo);
                break;
            // ... otros casos
        }
    } while (opcion != 3);
    
    if (nuevo != NULL) delete nuevo; // Liberar memoria
    return 0;
}
