#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "manager.h"
#include "worker.h"


int status;
pid_t *childs;
int size;

void on_sigabrt (int signum)
{
    printf("%d\n", size);
    for (int i = 0; i < size; i++)
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


void run_manager(InputFile* input_file, char* time_out, char* n_lines, char** line, bool root){
    pid_t child = 1;
    size = atoi(n_lines);
    childs = malloc(size);
    // Signal manage
    signal(SIGABRT, &on_sigabrt);
    if (root){
        signal(SIGINT, &on_sigabrt);
    }
    else{
        signal(SIGINT, SIG_IGN);
    }
    

    for (int i = 0; i < atoi(n_lines) && child != 0; i++)
    {
        printf("Loop\n");
        child = fork();
        childs[i] = child;
        if(child == 0){
            const int w = atoi(line[i+3]);
            if(*input_file -> lines[w][0] == 'W'){
                run_worker(input_file -> lines[w][1], input_file -> lines[w][2], input_file -> lines[w]);
            }
            else{
                run_manager(input_file, input_file -> lines[w][1], input_file -> lines[w][2], input_file -> lines[w], false);
                child = 0;
            }
        }
        // else
        // {
        //     waitpid(child,&status,0);
        // }
        
    }
    if (child != 0){
        puts("------------------ABORTING------------------");
        if(root){
            printf("Soy el root: %d\n", child);
        }
        else{
            printf("Soy el manager: %d\n", child);
        }
        sleep(20);
        // for (int i = 0; i < atoi(n_lines); i++)
        // {
        //     printf("Hijo: %d\n", childs[i]);
        // }
    }
        
}

void run_worker(char *file, char* n_args, char** args){
    worker();
};

