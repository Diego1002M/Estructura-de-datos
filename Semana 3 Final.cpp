#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// ----------------------- ESTRUCTURA BASE -----------------------
struct Proceso {
    int id;
    string nombre;
    string estado;
    int prioridad;
    float tiempoEjecucion;
    Proceso* siguiente;

    Proceso(int i, string n, int p, float t = 0.0f) 
        : id(i), nombre(n), estado("Listo"), prioridad(p), tiempoEjecucion(t), siguiente(NULL) {}
};

// ----------------------- LISTA ENLAZADA -----------------------
class ListaEnlazada {
private:
    Proceso* cabeza;
public:
    ListaEnlazada() : cabeza(NULL) {}
    
    ~ListaEnlazada() {
        Proceso* actual = cabeza;
        while (actual != NULL) {
            Proceso* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    bool buscarID(int id) {
        Proceso* temp = cabeza;
        while (temp != NULL) {
            if (temp->id == id) return true;
            temp = temp->siguiente;
        }
        return false;
    }

    void insertar(int id, string nombre, int prioridad, float tiempo = 0.0f) {
        if (buscarID(id)) {
            cout << "Error: ID ya existe.\n";
            return;
        }
        Proceso* nuevo = new Proceso(id, nombre, prioridad, tiempo);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    void mostrar() {
        Proceso* temp = cabeza;
        while (temp != NULL) {
            cout << "ID: " << temp->id 
                 << ", Nombre: " << temp->nombre
                 << ", Prioridad: " << temp->prioridad
                 << ", Estado: " << temp->estado << "\n";
            temp = temp->siguiente;
        }
    }

    Proceso* obtenerProceso(int id) {
        Proceso* temp = cabeza;
        while (temp != NULL) {
            if (temp->id == id) return temp;
            temp = temp->siguiente;
        }
        return NULL;
    }
};

// ----------------------- COLA DE PRIORIDAD -----------------------
class ColaCPU {
private:
    Proceso* frente;
public:
    ColaCPU() : frente(NULL) {}

    void encolar(Proceso* p) {
        if (frente == NULL || p->prioridad > frente->prioridad) {
            p->siguiente = frente;
            frente = p;
        } else {
            Proceso* actual = frente;
            while (actual->siguiente != NULL && 
                   actual->siguiente->prioridad >= p->prioridad) {
                actual = actual->siguiente;
            }
            p->siguiente = actual->siguiente;
            actual->siguiente = p;
        }
        p->estado = "En cola";
        cout << "Proceso " << p->id << " encolado (Prioridad: " << p->prioridad << ")\n";
    }

    void ejecutarProceso() {
        if (frente == NULL) {
            cout << "No hay procesos en cola.\n";
            return;
        }
        Proceso* ejecutado = frente;
        frente = frente->siguiente;
        ejecutado->estado = "Ejecutado";
        cout << "Ejecutando proceso ID: " << ejecutado->id << "\n";
        delete ejecutado;
    }
};

// ----------------------- PILA DE MEMORIA -----------------------
class PilaMemoria {
private:
    int* memoria;
    int tope;
    int capacidad;
public:
    PilaMemoria(int cap) : capacidad(cap), tope(-1) {
        memoria = new int[capacidad];
    }

    ~PilaMemoria() {
        delete[] memoria;
    }

    void asignar(int procesoID) {
        if (tope >= capacidad - 1) {
            cout << "Error: Memoria llena.\n";
            return;
        }
        memoria[++tope] = procesoID;
        cout << "Memoria asignada al proceso " << procesoID << " (Bloque " << tope << ")\n";
    }

    void liberar() {
        if (tope < 0) {
            cout << "Error: Memoria vacia.\n";
            return;
        }
        cout << "Liberando memoria del proceso " << memoria[tope--] << "\n";
    }
};

// ----------------------- PERSISTENCIA -----------------------
void guardarProcesos(ListaEnlazada& lista) {
    ofstream archivo("procesos.txt");
    Proceso* temp = lista.obtenerProceso(0); // Asume que existe este método
    while (temp != NULL) {
        archivo << temp->id << " " << temp->nombre << " " 
                << temp->estado << " " << temp->prioridad << " "
                << temp->tiempoEjecucion << "\n";
        temp = temp->siguiente;
    }
    archivo.close();
}

void cargarProcesos(ListaEnlazada& lista) {
    ifstream archivo("procesos.txt");
    if (!archivo.is_open()) return;

    int id, prioridad;
    string nombre, estado;
    float tiempo;

    while (archivo >> id >> nombre >> estado >> prioridad >> tiempo) {
        lista.insertar(id, nombre, prioridad, tiempo);
        Proceso* p = lista.obtenerProceso(id);
        if (p != NULL) p->estado = estado;
    }
    archivo.close();
}

// ----------------------- MENÚ INTERACTIVO -----------------------
void mostrarMenu() {
    cout << "\n=== SISTEMA DE GESTION DE PROCESOS ===" << endl;
    cout << "1. Agregar proceso" << endl;
    cout << "2. Mostrar todos los procesos" << endl;
    cout << "3. Encolar proceso (CPU)" << endl;
    cout << "4. Ejecutar proceso (CPU)" << endl;
    cout << "5. Asignar memoria" << endl;
    cout << "6. Liberar memoria" << endl;
    cout << "7. Guardar datos" << endl;
    cout << "8. Cargar datos" << endl;
    cout << "9. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

// ----------------------- MAIN -----------------------
int main() {
    ListaEnlazada lista;
    ColaCPU cola;
    PilaMemoria pila(5); // 5 bloques de memoria
    int opcion, id, prioridad;
    string nombre;
    float tiempo;

    cargarProcesos(lista); // Carga inicial

    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "ID del proceso: ";
                cin >> id;
                cout << "Nombre: ";
                cin.ignore();
                getline(cin, nombre);
                cout << "Prioridad (1-5): ";
                cin >> prioridad;
                cout << "Tiempo ejecucion (seg): ";
                cin >> tiempo;
                lista.insertar(id, nombre, prioridad, tiempo);
                break;

            case 2:
                lista.mostrar();
                break;

            case 3:
                cout << "ID del proceso a encolar: ";
                cin >> id;
                if (Proceso* p = lista.obtenerProceso(id)) {
                    cola.encolar(p);
                } else {
                    cout << "Proceso no encontrado.\n";
                }
                break;

            case 4:
                cola.ejecutarProceso();
                break;

            case 5:
                cout << "ID del proceso para memoria: ";
                cin >> id;
                pila.asignar(id);
                break;

            case 6:
                pila.liberar();
                break;

            case 7:
                guardarProcesos(lista);
                cout << "Datos guardados.\n";
                break;

            case 8:
                cargarProcesos(lista);
                cout << "Datos cargados.\n";
                break;

            case 9:
                cout << "Saliendo...\n";
                break;

            default:
                cout << "Opcion no valida.\n";
        }
    } while (opcion != 9);

    return 0;
}
