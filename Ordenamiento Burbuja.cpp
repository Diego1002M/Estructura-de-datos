void ordenamientoBurbuja(vector<Proceso>& procesos) {
    int n = procesos.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (procesos[j].id > procesos[j+1].id) {
                swap(procesos[j], procesos[j+1]);  // Intercambiar
            }
        }
    }
}

