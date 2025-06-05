#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Estructura base para procesos
struct Proceso {
    int id;
    string nombre;
    int prioridad;
    Proceso* siguiente;
    
    Proceso(int i, string n, int p) : id(i), nombre(n), prioridad(p), siguiente(NULL) {}
};

// 1. Lista Enlazada (Gestor de Procesos)
class ListaEnlazada {
private:
    Proceso* cabeza;
public:
    ListaEnlazada() : cabeza(NULL) {}
    
    bool existeID(int id) {
        Proceso* temp = cabeza;
        while (temp) {
            if (temp->id == id) return true;
            temp = temp->siguiente;
        }
        return false;
    }
    
    void insertar(int id, string nombre, int prioridad) {
        if (existeID(id)) {
            cout << "Error: ID ya existe\n";
            return;
        }
        Proceso* nuevo = new Proceso(id, nombre, prioridad);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        cout << "Proceso agregado!\n";
    }
    
    void mostrar() {
        Proceso* temp = cabeza;
        while (temp) {
            cout << "ID: " << temp->id << ", Nombre: " << temp->nombre 
                 << ", Prioridad: " << temp->prioridad << endl;
            temp = temp->siguiente;
        }
    }
    
    Proceso* obtenerPrimerProceso() const {
        return cabeza;
    }
};

// 2. Cola de Prioridad (Planificador de CPU)
class ColaCPU {
private:
    Proceso* frente;
public:
    ColaCPU() : frente(NULL) {}
    
    void encolar(Proceso* p) {
        if (!frente || p->prioridad > frente->prioridad) {
            p->siguiente = frente;
            frente = p;
        } else {
            Proceso* actual = frente;
            while (actual->siguiente && actual->siguiente->prioridad >= p->prioridad) {
                actual = actual->siguiente;
            }
            p->siguiente = actual->siguiente;
            actual->siguiente = p;
        }
        cout << "Proceso " << p->id << " encolado (Prioridad: " << p->prioridad << ")\n";
    }
};

// 3. Pila (Gestor de Memoria)
class PilaMemoria {
private:
    int* memoria;
    int tope;
    int capacidad;
public:
    PilaMemoria(int cap) : capacidad(cap), tope(-1) {
        memoria = new int[capacidad];
    }
    
    void asignar(int procesoID) {
        if (tope < capacidad - 1) {
            memoria[++tope] = procesoID;
            cout << "Memoria asignada al proceso " << procesoID << " (Bloque " << tope << ")\n";
        } else {
            cout << "Error: Memoria llena\n";
        }
    }
    
    void liberar() {
        if (tope >= 0) {
            cout << "Memoria liberada (Proceso " << memoria[tope] << ")\n";
            tope--;
        } else {
            cout << "Error: Memoria vacia\n";
        }
    }
};

// Persistencia de datos
void guardarProcesos(const ListaEnlazada& lista) {
    ofstream archivo("procesos.txt");
    Proceso* temp = lista.obtenerPrimerProceso();
    while (temp) {
        archivo << temp->id << "," << temp->nombre << "," << temp->prioridad << "\n";
        temp = temp->siguiente;
    }
    archivo.close();
    cout << "Datos guardados en 'procesos.txt'\n";
}

// Menú interactivo
void mostrarMenu() {
    cout << "\n=== SISTEMA DE GESTION ===" << endl;
    cout << "1. Agregar proceso" << endl;
    cout << "2. Mostrar procesos" << endl;
    cout << "3. Encolar proceso (CPU)" << endl;
    cout << "4. Asignar memoria" << endl;
    cout << "5. Liberar memoria" << endl;
    cout << "6. Guardar datos" << endl;
    cout << "7. Salir" << endl;
    cout << "Opcion: ";
}

int main() {
    ListaEnlazada lista;
    ColaCPU cola;
    PilaMemoria pila(5); // Memoria para 5 procesos
    int opcion, id, prioridad;
    string nombre;
    
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1: // Agregar proceso
                cout << "ID: "; cin >> id;
                cin.ignore();
                cout << "Nombre: "; getline(cin, nombre);
                cout << "Prioridad: "; cin >> prioridad;
                lista.insertar(id, nombre, prioridad);
                break;
                
            case 2: // Mostrar procesos
                lista.mostrar();
                break;
                
            case 3: // Encolar en CPU
                cout << "ID del proceso a encolar: "; cin >> id;
                {
                    Proceso* temp = lista.obtenerPrimerProceso();
                    while (temp && temp->id != id) {
                        temp = temp->siguiente;
                    }
                    if (temp) {
                        cola.encolar(temp);
                    } else {
                        cout << "Error: Proceso no encontrado en la lista\n";
                    }
                }
                break;
                
            case 4: // Asignar memoria
                cout << "ID del proceso para memoria: "; cin >> id;
                pila.asignar(id);
                break;
                
            case 5: // Liberar memoria
                pila.liberar();
                break;
                
            case 6: // Guardar
                guardarProcesos(lista);
                break;
                
            case 7: // Salir
                cout << "Saliendo...\n";
                break;
                
            default:
                cout << "Opcion invalida!\n";
        }
    } while (opcion != 7);
    
    return 0;
}

