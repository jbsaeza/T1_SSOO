#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "manager.h"
#include "worker.h"


pid_t child = 1;
void run_manager(InputFile* input_file, char* time_out, char* n_lines, char** line, bool root){
    pid_t childs[atoi(n_lines)];

    // Signal manage
    void on_sigabrt (int signum)
    {
        for (int i = 0; i < atoi(n_lines); i++)
        {
            printf("Hijo: %d\n", childs[i]);
            kill(childs[i], SIGABRT);
            // Ver como esperar la info de los hijos q no han terminado
            // Hacer childs con memoria compartida? o asignar memoria que los hijos vayan rellenando
            // data = wait(childs[i]);
        }
        // En vez de esto se deberia agregar la data recolectada en el archivo y luego hacer exit
        // abort();
    }
    signal(SIGABRT, &on_sigabrt);
    if (root){
        signal(SIGINT, &on_sigabrt);
    }
    else{
        signal(SIGINT, SIG_IGN);
    }
    

    printf("Time out: %s\n", time_out);
    printf("N lineas: %s\n", n_lines);
    for (int i = 0; i < atoi(n_lines) && child != 0; i++)
    {
        printf("Loop\n");
        child = fork();
        childs[i] = child;
        if(child == 0){
            const int w = atoi(line[i+3]);
            printf("Tipo de linea: %s\n", input_file -> lines[w][0]);
            if(*input_file -> lines[w][0] == 'W'){
                printf("Ejecutando Worker debido a managar o root: %s\n", input_file -> lines[w][1]);
                run_worker(input_file -> lines[w][1], input_file -> lines[w][2], input_file -> lines[w]);
            }
            else{
                puts("Ejecutando Manager");
                run_manager(input_file, input_file -> lines[w][1], input_file -> lines[w][2], input_file -> lines[i], false);
            }
            
            // printf("Linea worker: %d\n", w);
            // printf("Process: %s\n", input_file -> lines[w][1]);
            // printf("N args: %s\n", input_file -> lines[w][2]);
        }
    }
    // if (child != 0){
    //     // sleep(3);
    //     // puts("------------------ABORTING------------------");
    //     // abort();
    //     // printf("Soy el manager\n");
    // }
        
}

void run_worker(char *file, char* n_args, char** args){
    worker();
};

