int busquedaBinaria(vector<Proceso>& procesos, int idBuscado) {
    int inicio = 0, fin = procesos.size() - 1;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        if (procesos[medio].id == idBuscado) {
            return medio;
        } else if (procesos[medio].id < idBuscado) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }
    return -1;
}


