#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../file_manager/manager.h"

/* Interrupt the process */ 
#define SIGINT  2 

/* Abort. */
#define SIGABRT 6

// int getSize (char** s) 
// {
//   char** t;    
//   for (t = s; *t != '\0'; t++)
//     ;
//   return t - s;
// }

void worker(char** process)
{
  int n = atoi(process[2]);
  char** args = malloc( (n + 2) * sizeof(char*)); /* Se pide memoria para la cantida de procesos mas el archivo  y el puntero final null*/
  args[0] = process[1];
  for (int i = 0; i < n; i++)
  {
    args[ i + 1] = process[3 + i];
  }
  args[ n + 1] = NULL;
  if (fork() != 0)
  {
    // waitpid(-1, &status, 0);
    printf("Ejecutando Padre\n");
    wait(NULL);
    printf("Hijo Termino\n");
  } 
  else 
  {
    // execve(file, parameters, 0);
    printf("Ejecutando hijo\n");
    execve(args[0], args, 0);
  } 
}

int main(int argc, char **argv)
{
  InputFile* input_file = read_file(argv[1]);
  int process = atoi(argv[2]);
  for (int i = process; i < input_file -> len; i++) {
    switch (input_file -> lines[i][0][0])
    {
    case 'W':
      printf("Ejecutando Worker\n");
      worker(input_file -> lines[i]);
      break;

    case 'M':
      printf("Ejecutando Manager\n");
      break;

    case 'R':
      printf("Ejecutando Root\n");
    }
  }
  input_file_destroy(input_file);
  return 0;
}
