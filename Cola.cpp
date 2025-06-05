//Cola

void ColaPrioridad::encolar(Proceso* proceso) {
    procesos.push_back(proceso);  // Añadir al final
    // Ordenar por prioridad ascendente
    sort(procesos.begin(), procesos.end(), [](Proceso* a, Proceso* b) {
        return a->prioridad < b->prioridad;
    });
}


