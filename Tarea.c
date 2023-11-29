#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define MAX_TUNEL_CAPACIDAD 1 // Capacidad máxima del túnel
#define NUM_TUNEL 4 // Cantidad de túneles
#define BUFFER_SIZE 256 // Tamaño del buffer

int entradaAC[2];
int entradaCB[2];
int entradaDE[2];
int entradaED[2];


// Estructura para el monitor del túnel
typedef struct{
    pthread_mutex_t mutex;
    int capacidad; //capacidad del tunel
    int cant_autos; //Cantidad de autos que cruzan en el tunel
}MonitorTunel;

// Estructura para los datos de un automóvil
typedef struct{
    int id;
    int prioridad; 
}Automovil;

// Inicializando el monitor del tunel
void initMonitorTunel(MonitorTunel *monitor){
    
    monitor->capacidad=MAX_TUNEL_CAPACIDAD;
    monitor->cant_autos=0;
    pthread_mutex_init(&monitor->mutex, NULL);

}

// Acceso al Monitor
void accesoMonitor(MonitorTunel *monitor){

    pthread_mutex_lock(&monitor->mutex);

}

// Salida del Monitor
void salirMonitor(MonitorTunel *monitor){

    pthread_mutex_unlock(&monitor->mutex);
    
}

// Función para que el automóvil envíe un mensaje al monitor para entrar al túnel
void cruzaVehiculoAC(MonitorTunel *monitor, Automovil *Auto) {
    accesoMonitor(monitor);
    char msj[] = "Mensaje desde Entrada A";
}

// Función para que el automóvil notifique al monitor cuando sale del túnel
void SaleVehiculo(MonitorTunel *monitor, Automovil *Auto) {

}

// Creacion de funciones de los hilos que simulan cada entrada

// Entrada A
void *hiloEntradaA(MonitorTunel *monitor, Automovil *Auto){

}







// Función para manejar la señal de emergencia
void emergencia(int adelanto) {

}




int main() {

    return 0;
}