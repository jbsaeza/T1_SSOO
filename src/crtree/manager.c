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


int *status;
pid_t *children;
int *children_index;
int size;
int manager_number;

void free_worker(){
    free(children);
    free(children_index);
}

void free_manager(){
    free(children);
    free(children_index);
}

void generate_output()
{
    FILE* child_file;
    size_t len;
    ssize_t read;
    char* line;
    char* buffer = malloc(255*sizeof(char));
    sprintf(buffer,"./%i.txt",manager_number);
    FILE* file = fopen(buffer,"w");
    for (int i = 0; i < size; i++)
    {
        char* buffer_child = malloc(255*sizeof(char));
        line = NULL;
        len = 0;
        sprintf(buffer_child,"./%i.txt", children_index[i]);
        child_file = fopen(buffer_child, "r");
        if (child_file == NULL)
            exit(EXIT_FAILURE);
        while((read = getline(&line, &len, child_file)) != -1)
        {
            fputs(line, file);
        }
        fclose(child_file);
        if (line) free(line);
        free(buffer_child);
    }
    fclose(file);
    free(buffer);
    free(children);
    free(children_index);
    free(status);
}



void on_sigabrt (int signum)
{
    for (int i = 0; i < size; i++)
    {
        kill(children[i], SIGABRT);
    }
    for (int i = 0; i < size; i++)
    {
        waitpid(children[i], &status[i], 0);
    }
    generate_output();
    return;
}


void run_manager(InputFile* input_file, char* time_out, char* n_lines, char** line, bool root, int index){
    time_t start = time(NULL);
    pid_t child = 1;
    size = atoi(n_lines);
    children = malloc(size * sizeof(pid_t));
    children_index = malloc(size * sizeof(int));
    manager_number = index;

    for (int i = 0; i < atoi(n_lines) && child != 0; i++)
    {
        child = fork();
        if (child != 0)
        {
            printf("-----M: %d\n", getpid());
            children[i] = child;
            children_index[i] = atoi(line[i+3]);
        }
        else{
            printf("-----%s: %d\n", input_file -> lines[atoi(line[i+3])][0], getpid());
            const int w = atoi(line[i+3]);
            if(*input_file -> lines[w][0] == 'W'){
                worker(input_file -> lines[w], w);
                free_worker();
                return;
            }
            else{
                run_manager(input_file, input_file -> lines[w][1], input_file -> lines[w][2], input_file -> lines[w], false, w);
                // free_manager();
                return;
            }
        }
    }
    if (child != 0){
        // Signal manage
        signal(SIGABRT, &on_sigabrt);
        if (root){
            signal(SIGINT, &on_sigabrt);
        }
        else{
            signal(SIGINT, SIG_IGN);
        }
        status = malloc(size * sizeof(int));
        while (time(NULL)-start < atoi(time_out)){
            int finished = 0;
            for (int i = 0; i < atoi(n_lines); i++)
            {
                waitpid(children[i], &status[i], WNOHANG);
                if (status[i] == 0)
                {
                    finished++;
                }                
            }
            if (finished == size)
            {
                generate_output();
                return;
            }
        }
        kill(getpid(), SIGABRT);
    }
}
