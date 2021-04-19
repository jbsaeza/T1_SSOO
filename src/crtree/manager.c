#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "manager.h"
#include "worker.h"


int status;
pid_t *children;
char *children_index;
int size;
int manager_number;
void generate_output()
{
  FILE* child_file;
  size_t len;
  ssize_t read;
  char* line;
  char* buffer = malloc(100*sizeof(char));
  sprintf(buffer,"./i.txt",manager_number);
  FILE* file = fopen(buffer,"w");
  for (int i = 0; i < size; i++)
  {
    line = NULL;
    len = 0;
    sprintf(buffer,"./i.txt",children_index[i]);
    child_file = fopen(buffer,"r");
    while((read = getline(&line, &len, child_file)) != -1)
    {
      fputs(line, file);/*Hay que ver si agregar un \n*/
    }
    fclose(child_file);
    if (line) free(line);
  }
  fclose(file);
  free(buffer);
//   int main(void)
//     {
//         FILE * fp;
//         char * line = NULL;
//         size_t len = 0;
//         ssize_t read;

//         fp = fopen("/etc/motd", "r");
//         if (fp == NULL)
//             exit(EXIT_FAILURE);

//         while ((read = getline(&line, &len, fp)) != -1) {
//             printf("Retrieved line of length %zu:\n", read);
//             printf("%s", line);
//         }

//         fclose(fp);
//         if (line)
//             free(line);
//         exit(EXIT_SUCCESS);
//     }
}



void on_sigabrt (int signum)
{
    printf("%d\n", size);
    for (int i = 0; i < size; i++)
    {
        printf("Hijo: %d\n", children[i]);
        kill(children[i], SIGABRT);
        // Ver como esperar la info de los hijos q no han terminado
        // Hacer children con memoria compartida? o asignar memoria que los hijos vayan rellenando
        // data = wait(children[i]);
    }
    // En vez de esto se deberia agregar la data recolectada en el archivo y luego hacer exit
    // abort();
}


void run_manager(InputFile* input_file, char* time_out, char* n_lines, char** line, bool root, int index){
    time_t start = time(NULL);
    pid_t child = 1;
    size = atoi(n_lines);
    children = malloc(size);
    children_index = malloc(size);
    manager_number = index;
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
        children[i] = child;
        children_index[i] = *line[i+3];
        if(child == 0){
            const int w = atoi(line[i+3]);
            if(*input_file -> lines[w][0] == 'W'){
                run_worker(input_file -> lines[w][1], input_file -> lines[w][2], input_file -> lines[w]);
            }
            else{
                run_manager(input_file, input_file -> lines[w][1], input_file -> lines[w][2], input_file -> lines[w], false, w);
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
        while (time(NULL)-start < atoi(time_out)){
            for (int i = 0; i < atoi(n_lines); i++)
            {
                printf("Esperando a Hijo: %d\n", children[i]);
                printf("Esperando a Hijo de indice: %c\n", children_index[i]);
                waitpid(children[i], &status, WNOHANG);
            }
        }
        abort();
        // sleep(20);
    }
        
}

void run_worker(char *file, char* n_args, char** args){
    worker();
};

