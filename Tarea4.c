// Dieter Heim Trujillo - Leandro Neilaf

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define BUFFER_SIZE 256 // Tamaño del buffer
#define NUM_VEH_A 3 //Cantidad de vehiculos que entran al tunel AC
#define NUM_VEH_C 4 //Cantidad de vehiculos que entran al tunel CB
#define NUM_VEH_D 5 //Cantidad de vehiculos que entran al tunel DE

/*-------------------- Variables --------------------*/
int entradaAV[2];
int entradaVA[2];

int entradaBV[2];
int entradaVB[2];

int entradaCV[2];
int entradaVC[2];

int entradaDV[2];
int entradaVD[2];

// Mutex para pipes
pthread_mutex_t respuestaA,respuestaC, respuestaD;


// Estructura para el monitor del túnel
typedef struct{
    pthread_mutex_t mutex;
    int cant_autos; //Cantidad de autos que cruzan en el tunel
}MonitorTunel;
/*-------------------- Variables --------------------*/


/*-------------------- Funciones --------------------*/
// Inicializando el monitor del tunel
void initMonitorTunel(MonitorTunel *monitor){
    
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
// Función para que el automóvil notifique al monitor cuando sale del túnel
void saleVehiculo(MonitorTunel *monitor, pthread_t id, char tunel[]) {
    printf("Vehiculo %lu sale del tunel %s\n", id, tunel);
    salirMonitor(monitor);
    monitor->cant_autos=0;
    pthread_exit(NULL);
}

// Función para que el automóvil envíe un mensaje al monitor para entrar al túnel
void ingresaVehiculo(MonitorTunel *monitor, pthread_t id, char tunel[]) {
    accesoMonitor(monitor);
    monitor->cant_autos=1;
    printf("Vehiculo %lu ingresando a Tunel %s\n", id, tunel);
    saleVehiculo(monitor, id, tunel);
}
/*-------------------- Funciones --------------------*/


/*----------------Funciones de Hilos Vehiculos----------------*/
//Hilos vehiculo A
void *hiloVehiculoA(MonitorTunel *monitor) {
    //Mensaje a envia desde Vehiculo a monitor entrada A
    pthread_t idVehiculo = pthread_self();
    char tunel[]="AC";
    pthread_mutex_lock(&respuestaA);
    char msj[] = "Solicitud ingreso de vehiculo a entrada A";
    printf("Vehiculo %lu: Enviando mensaje a Entrada A\n", idVehiculo);
    
    // Escribir en el pipe de Vehiculo a Canal A
    write(entradaVA[1], msj, sizeof(msj));

    char buffer[BUFFER_SIZE];
    read(entradaAV[0], buffer, sizeof(buffer));
    printf("Vehiculo %lu: Mensaje recibido desde Entrada A: %s\n", idVehiculo, buffer);
    pthread_mutex_unlock(&respuestaA);

    if (monitor->cant_autos==0){
        ingresaVehiculo(monitor, idVehiculo, tunel);

    }else if (monitor->cant_autos==1)
    {
        printf("Vehiculo %lu tiene que esperar para cruzar el tunel %s\n", idVehiculo, tunel);

    }
    sleep(2);

    pthread_exit(NULL);
}

//Hilos vehiculo C
void *hiloVehiculoC(MonitorTunel *monitor) {
    //Mensaje a envia desde Vehiculo a monitor entrada C
    pthread_t idVehiculo = pthread_self();
    char tunel[]="CB";
    pthread_mutex_lock(&respuestaC);
    char msj[] = "Solicitud ingreso de vehiculo a entrada C";
    printf("Vehiculo %lu: Enviando mensaje a entrada C\n", idVehiculo);
    
    // Escribir en el pipe de Vehiculo a Canal C
    write(entradaVC[1], msj, sizeof(msj));

    char buffer[BUFFER_SIZE];
    read(entradaCV[0], buffer, sizeof(buffer));
    printf("Vehiculo %lu: Mensaje recibido desde Entrada C: %s\n", idVehiculo, buffer);
    pthread_mutex_unlock(&respuestaC);

    if (monitor->cant_autos==0){
        ingresaVehiculo(monitor, idVehiculo, tunel);

    }else if (monitor->cant_autos==1)
    {
        printf("Vehiculo %lu tiene que esperar para cruzar el tunel %s\n", idVehiculo, tunel);

    }
    sleep(2);

    pthread_exit(NULL);
}

//Hilos vehiculo D
void *hiloVehiculoD(MonitorTunel *monitor) {
    //Mensaje a envia desde Vehiculo a monitor entrada D
    pthread_t idVehiculo = pthread_self();
    char tunel[]="DE";
    pthread_mutex_lock(&respuestaD);
    char msj[] = "Solicitud ingreso de vehiculo a entrada D";
    printf("Vehiculo %lu: Enviando mensaje a Entrada D\n", idVehiculo);
    
    // Escribir en el pipe de Vehiculo a Canal D
    write(entradaVD[1], msj, sizeof(msj));

    char buffer[BUFFER_SIZE];
    read(entradaDV[0], buffer, sizeof(buffer));
    printf("Vehiculo %lu: Mensaje recibido desde Entrada D: %s\n", idVehiculo, buffer);
    pthread_mutex_unlock(&respuestaD);

    if (monitor->cant_autos==0){
        ingresaVehiculo(monitor, idVehiculo, tunel);

    }else if (monitor->cant_autos==1)
    {
        printf("Vehiculo %lu tiene que esperar para cruzar el tunel %s\n", idVehiculo, tunel);

    }
    sleep(2);

    pthread_exit(NULL);
}
/*----------------Funciones de Hilos Vehiculos----------------*/


/*----------------Funciones de Hilos Tuneles----------------*/
// Hilo Tunel AC
void *hiloEntradaA(MonitorTunel *monitor){
    while(1){
        
        char buffer[BUFFER_SIZE];
        read(entradaVA[0], buffer, sizeof(buffer));
        printf("Entrada A: Mensaje recibido desde el Vehiculo: %s\n", buffer);

        char reply1[] = "Se puede cruzar";
        char reply2[] = "No se puede cruzar";

        printf("Entrada A: Enviando respuesta al Vehiculo\n");
        if (monitor->cant_autos==0){
            write(entradaAV[1], reply1, sizeof(reply1));
        }else if (monitor->cant_autos==1){
            write(entradaAV[1], reply2, sizeof(reply2));
        }
    }
    pthread_exit(NULL);
}

// Hilo Tunel CB
void *hiloEntradaC(MonitorTunel *monitor){
    while(1){
        
        char buffer[BUFFER_SIZE];
        read(entradaVC[0], buffer, sizeof(buffer));
        printf("Entrada C: Mensaje recibido desde el Vehiculo: %s\n", buffer);

        char reply1[] = "Se puede cruzar";
        char reply2[] = "No se puede cruzar";

        printf("Entrada C: Enviando respuesta al Vehiculo\n");
        if (monitor->cant_autos==0){
            write(entradaCV[1], reply1, sizeof(reply1));
        }else if (monitor->cant_autos==1){
            write(entradaCV[1], reply2, sizeof(reply2));
        }
    }
    pthread_exit(NULL);
}

// Hilo Tunel DE
void *hiloEntradaD(MonitorTunel *monitor){
    while(1){
        
        char buffer[BUFFER_SIZE];
        read(entradaVD[0], buffer, sizeof(buffer));
        printf("Entrada D: Mensaje recibido desde el Vehiculo: %s\n", buffer);

        char reply1[] = "Se puede cruzar";
        char reply2[] = "No se puede cruzar";

        printf("Entrada D: Enviando respuesta al Vehiculo\n");
        if (monitor->cant_autos==0){
            write(entradaDV[1], reply1, sizeof(reply1));
        }else if (monitor->cant_autos==1){
            write(entradaDV[1], reply2, sizeof(reply2));
        }
    }
    pthread_exit(NULL);
}
/*----------------Funciones de Hilos Tuneles----------------*/


/*-------------------- Funcion Main --------------------*/

int main() {
    // Creacion de monitores
    MonitorTunel monitorAC, monitorCB, monitorDE;
    initMonitorTunel(&monitorAC);
    initMonitorTunel(&monitorCB);
    initMonitorTunel(&monitorDE);

    // Creacion de los pipes
    if (pipe(entradaAV) == -1 || pipe(entradaVA) == -1){
        perror("Error al crear los pipes");
        exit(EXIT_FAILURE);
    }
    if (pipe(entradaCV) == -1 || pipe(entradaVC) == -1){
        perror("Error al crear los pipes");
        exit(EXIT_FAILURE);
    }
    if (pipe(entradaDV) == -1 || pipe(entradaVD) == -1){
        perror("Error al crear los pipes");
        exit(EXIT_FAILURE);
    }
    // Inicializacion de Mutex
    pthread_mutex_init(&respuestaA, NULL);
    pthread_mutex_init(&respuestaC, NULL);
    pthread_mutex_init(&respuestaD, NULL);

    // Declaracion de hilos
    pthread_t hiloA, hiloC, hiloD;
    pthread_t hilosVehiculosA[NUM_VEH_A], hilosVehiculosC[NUM_VEH_C], hilosVehiculosD[NUM_VEH_D];


    // Creacion hilos de tuneles
    pthread_create(&hiloA, NULL, (void*(*)(void *))hiloEntradaA, &monitorAC);
    pthread_create(&hiloC, NULL, (void*(*)(void *))hiloEntradaC, &monitorCB);
    pthread_create(&hiloD, NULL, (void*(*)(void *))hiloEntradaD, &monitorDE);


    // Creaci[on hilos de vehiculos
    for(int i = 0; i < NUM_VEH_A; i++){
        pthread_create(&hilosVehiculosA[i], NULL, (void*(*)(void *))hiloVehiculoA, &monitorAC);
    }
    for(int i = 0; i < NUM_VEH_C; i++){
        pthread_create(&hilosVehiculosC[i], NULL, (void*(*)(void *))hiloVehiculoC, &monitorCB);
    }
    for(int i = 0; i < NUM_VEH_D; i++){
        pthread_create(&hilosVehiculosD[i], NULL, (void*(*)(void *))hiloVehiculoD, &monitorDE);
    }


    // Espera a que los hilos terminen
    for(int i = 0; i < NUM_VEH_A; i++){
        pthread_join(hilosVehiculosA[i], NULL);
    }
    for(int i = 0; i < NUM_VEH_C; i++){
        pthread_join(hilosVehiculosC[i], NULL);
    }
    for(int i = 0; i < NUM_VEH_D; i++){
        pthread_join(hilosVehiculosD[i], NULL);
    }


    // Destruyendo los mutex
    pthread_mutex_destroy(&monitorAC.mutex);
    pthread_mutex_destroy(&monitorCB.mutex);
    pthread_mutex_destroy(&monitorDE.mutex);
    pthread_mutex_destroy(&respuestaA);
    pthread_mutex_destroy(&respuestaC);
    pthread_mutex_destroy(&respuestaD);


    return 0;
}
/*-------------------- Funcion Main --------------------*/
