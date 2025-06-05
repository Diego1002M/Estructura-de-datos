//Inserta
void ListaEnlazada::insertarProceso(int id, string nombre, int prioridad) {
    Proceso* nuevo = new Proceso(id, nombre, prioridad);
    if (!cabeza || cabeza->id >= nuevo->id) {
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    } else {
        Proceso* actual = cabeza;
        while (actual->siguiente && actual->siguiente->id < nuevo->id) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }
}


