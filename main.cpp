#include <omp.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "datos.h"
#include "bubblesort.h"

using namespace std;

void mostrarDatos(vector<Persona> datos) {
    size_t size = datos.size();
    for (size_t i = 0; i < size; i++)
    {
        printf("NOMBRE: %s         EDAD: %d\n", datos[i].nombre.c_str(), datos[i].edad);
    }
}

int valorMaximo(vector<Persona> datos) {
    int valorMaximo = datos[0].edad;
    size_t size = datos.size();
    for (size_t i = 0; i < size; i++)
    {
        if(valorMaximo < datos[i].edad) {
            valorMaximo = datos[i].edad;
        }
    }
    return valorMaximo;
}

vector<Persona> bucketSort(vector<Persona> datos, int numBuckets, int numThreads) {
    
    if (numBuckets <= 0)
    {
        return datos;
    }

    //Reservacion de memoria para los bloques
    vector<Persona> buckets[numBuckets];

    //Reparticion de elementos a los bloques segun su clasificacion
    int rango = valorMaximo(datos) / numBuckets;
    if (rango == 0)
    {
        rango = 1;
    }
    int size = datos.size();
    for (int i = 0; i < size; i++)
    {
        int dato = datos[i].edad;
        int idx = dato / rango;
        if (idx > numBuckets - 1)
        {
            idx -= idx - (numBuckets - 1); 
        }
        buckets[idx].push_back(datos[i]);
    }
    
    //Ordenacion concurrente de los bloques
    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < numBuckets; i++)
    {
        buckets[i] = bubbleSort(buckets[i]);
    }

    datos.clear();
    for (int i = 0; i < numBuckets; i++)
    {
        datos.insert(datos.end(), buckets[i].begin(), buckets[i].end());
    }

    return datos;    
}

int main() {
    double tiempoInicial;
    double tiempoFinal;
    double tiempoSecuencial;
    double tiempoParalelo;

    vector<Persona> datos = getPersonas();
    printf("\n\n--------------------VECTOR ORIGINAL--------------------\n\n");
    
    mostrarDatos(datos);

    printf("\n\n--------------------VECTOR SECUENCIAL--------------------\n\n");
    
    tiempoInicial = omp_get_wtime();
    vector<Persona> secuencial = bubbleSort(datos);
    tiempoFinal = omp_get_wtime();
    tiempoSecuencial = tiempoFinal - tiempoInicial;
    
    mostrarDatos(secuencial);

    printf("\n\n--------------------VECTOR PARALELO--------------------\n\n");
    int numBuckets = 20;
    int numThreads = 4;

    tiempoInicial = omp_get_wtime();
    vector<Persona> paralelo = bucketSort(datos, numBuckets, numThreads);
    tiempoFinal = omp_get_wtime();
    tiempoParalelo = tiempoFinal - tiempoInicial;

    mostrarDatos(paralelo);

    printf("\n\n--------------------CALCULOS--------------------\n\n");
    printf("TIEMPO SECUENCIAL: %lf\n", tiempoSecuencial);
    printf("TIEMPO PARALELO: %lf\n", tiempoParalelo);
    printf("SPEEDUP: %lf\n", tiempoSecuencial / tiempoParalelo);
}