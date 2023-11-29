#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define MAX_TUNEL_CAPACIDAD 1 // Capacidad máxima del túnel
#define NUM_TUNEL 4 // Cantidad de túneles
#define BUFFER_SIZE 256 // Tamaño del buffer

int entradaA[2];
int entradaB[2];
int entradaC[2];
int entradaD[2];
int entradaE[2];


// Estructura para el monitor del túnel
typedef struct{
    pthread_mutex_t mutex;
    int capacidad; //capacidad del tunel
    int cant_autos; //Cantidad de autos que cruzan en el tunel
}MonitorTunel;

// Estructura para los datos de un automóvil
typedef struct{
    int id;
    int prioridad; // 0 = civil -- 1 = Emergencia
}Vehiculo;

// Inicializando el monitor del tunel
void initMonitorTunel(MonitorTunel *monitor){
    
    monitor->capacidad=MAX_TUNEL_CAPACIDAD;
    monitor->cant_autos=0;
    pthread_mutex_init(&monitor->mutex, NULL);

}

// Inicializando el vehiculo
void initVehiculo(Vehiculo *vehiculo, int identificador, int prio){

    vehiculo->id=identificador;
    vehiculo->prioridad=prio;

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
void ingresaVehiculoAC(MonitorTunel *monitor, Vehiculo *vehiculo) {
    
    //Mensaje a envia desde Vehiculo a monitor entrada A
    char msj[] = "Solicitud ingreso de vehiculo a Entrada A";
    printf("Vehiculo %d: Enviando mensaje a Entrada A", vehiculo->id);


    accesoMonitor(monitor);
}

// Función para que el automóvil notifique al monitor cuando sale del túnel
void saleVehiculo(MonitorTunel *monitor, Vehiculo *vehiculo) {

}





int main() {

    return 0;
}