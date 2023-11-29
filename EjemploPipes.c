// PROBLEMA DE LOS CANALES DE COMUNICACIÓN  (UTILIZANDO PIPES)

/* Actualmente existe un problema de conexión en dos canales de comunicación: Canal A y Canal B.
Cada canal tiene la tarea de enviar un mensaje al otro y recibir una respuesta. El objetivo es asegurar
que ambos canales se comuniquen correctamente. La tarea consiste en codificar la solución, y asegurarse
de que el mensaje enviado por el Canal A sea recibido y respondido por el Canal B, y viceversa.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 256 // tamaño del Buffer

int canalAB[2];
int canalBA[2];

// Función del hilo que simula el Canal A
void *hilo_canalA()
{
    // Mensaje a enviar desde el Canal A
    char msj[] = "Mensaje desde el Canal A";
    printf("Canal A: Enviando mensaje al Canal B\n");

    // Escribir en el pipe de Canal A a Canal B
    write(canalAB[1], msj, sizeof(msj));

    // Leer desde el pipe de Canal B a Canal A
    char buffer[BUFFER_SIZE];
    read(canalBA[0], buffer, sizeof(buffer));
    printf("Canal A: Mensaje recibido desde el Canal B: %s\n", buffer);

    pthread_exit(NULL);
}

// Función del hilo que simula el Canal B
void *hilo_canalB()
{
    // Leer desde el pipe de Canal A a Canal B
    char buffer[BUFFER_SIZE];
    read(canalAB[0], buffer, sizeof(buffer));
    printf("Canal B: Mensaje recibido desde el Canal A: %s\n", buffer);

    // Respuesta desde el Canal B
    char reply[] = "Respuesta desde el Canal B";
    printf("Canal B: Enviando respuesta al Canal A\n");

    // Escribir en el pipe de Canal B a Canal A
    write(canalBA[1], reply, sizeof(reply));

    pthread_exit(NULL);
}

int main()
{
    // Creación de los pipes
    if (pipe(canalAB) == -1 || pipe(canalBA) == -1)
    {
        perror("Error al crear los pipes");
        exit(EXIT_FAILURE);
    }

    pthread_t canalA_ID, canalB_ID;

    // Crear hilos para los túneles
    if (pthread_create(&canalA_ID, NULL, hilo_canalA, NULL) != 0 ||
        pthread_create(&canalB_ID, NULL, hilo_canalB, NULL) != 0)
    {
        perror("Error al crear los hilos");
        exit(EXIT_FAILURE);
    }

    // Esperando a que ambos hilos terminen
    pthread_join(canalA_ID, NULL);
    pthread_join(canalB_ID, NULL);

    // Cerrar los extremos de los pipes que ya no son necesarios
    close(canalAB[0]);
    close(canalAB[1]);
    close(canalBA[0]);
    close(canalBA[1]);

    return 0;
}