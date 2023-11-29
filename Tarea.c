#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define MAX_TUNEL_CAPACIDAD 1 // Capacidad máxima del túnel
#define NUM_TUNEL 5 // Cantidad de túneles

// Estructura para el monitor del túnel
typedef struct{
    pthread_mutex_t mutex;
    int capacidad; 
    int cant_autos; 
}MonitorTunel;

// Estructura para los datos de un automóvil
typedef struct{
    int id;
    int prioridad; 
}Automovil;

// Función para manejar la señal de emergencia
void emergencia(int adelanto) {

}

// Función para que el automóvil envíe un mensaje al monitor para entrar al túnel
void EntraVehiculor(MonitorTunel monitor, Automovil Auto) {

}

// Función para que el automóvil notifique al monitor cuando sale del túnel
void SaleVehiculo(MonitorTunel monitor, Automovil Auto) {

}

// Función ejecutada por cada hilo de automóvil
void HiloAutomovil(voidarg) {
    Automovil Auto = (Automovil)arg;

}

int main() {

    return 0;
}