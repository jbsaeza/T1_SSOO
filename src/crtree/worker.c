/* 
El proceso worker recibe un ejecutable, el numero de argumentos y luego los argumentos

Ejecuta el programa que recibe como input y lo espera, luego retorna estadisticas en un archivo
Debe interceptar señales sigint y sigabort (la primera se ignora y la segunda es de interrupcion,
indica que el proceso padre debe escribir las estadisticas antes de que el hijo termine)


*/

// void worker(char *file, int n, char *args[])
// {
//   if (fork() != 0)
//   {
//     waitpid(-1, &status, 0);
//   } 
//   else 
//   {
//     execve(file, parameters, 0);
//   } 
// }

// int getSize (char** s) {
//     char** t;    
//     for (t = s; *t != '\0'; t++)
//         ;
//     return t - s;
// }

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void worker(){
    void on_sigabrt (int signum)
    {
        puts("WORKER ABORTED");
        // En vez de esto deberia ser un exit con la data pal archivo
        signal(SIGABRT, SIG_DFL);
        abort();
    }
    signal(SIGABRT, &on_sigabrt);

    // sleep(5);
    puts("Worker terminado");
    
}