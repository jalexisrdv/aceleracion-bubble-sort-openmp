#include <iostream>
#include <vector>

using namespace std;

vector<Persona> bubbleSort(vector<Persona> datos) {
    Persona persona = datos[0];
    int size = datos.size();
    for(int i=0; i<size; i++) {
        for(int j=i+1; j<size; j++) {
            if(datos[i].edad>datos[j].edad) {
                persona=datos[i];
                datos[i]=datos[j];
                datos[j]=persona;
            }
        }
    }
    return datos;
}