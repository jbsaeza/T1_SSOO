#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include "../file_manager/manager.h"

/* Interrupt the process */ 
#define SIGINT  2 

/* Abort. */
#define SIGABRT 6

void handle_signal(int sig)
{
  printf("Caught signal %d\n", sig);
}

void worker(char** process, int number)
{
  int status;
  clock_t tiempo_inicio, tiempo_final;
  FILE* file;

  char* buffer = malloc(100*sizeof(char));
  int n = atoi(process[2]);
  /* Se pide memoria para la cantida de procesos mas el archivo  y el puntero final null*/
  char** args = malloc( (n + 2) * sizeof(char*));
  args[0] = process[1];
  for (int i = 0; i < n; i++)
  {
    args[ i + 1] = process[3 + i];
  }
  args[ n + 1] = NULL;
  tiempo_inicio = clock();
  // signal();
  if (fork() != 0)
  {
    waitpid(-1,&status,0);
    tiempo_final = clock();
    sprintf(buffer,"./%i.txt",number);
    file = fopen(buffer,"w");
    sprintf(buffer, "%s,args...,%f,%i,interrumped or not\n",process[1],(float)(tiempo_final - tiempo_inicio),status);
    fputs(buffer,file);
    free(buffer);
    free(args);
  } 
  else 
  {
    execve(args[0], args, 0);
  }
}

int main(int argc, char **argv)
{
  InputFile* input_file = read_file(argv[1]);
  int process = atoi(argv[2]);
  switch (input_file -> lines[process][0][0])
  {
    case 'W':
      worker(input_file -> lines[process], process);
      break;

    case 'M':
      break;

    case 'R':
      break;
  }
  input_file_destroy(input_file);
  return 0;
}
