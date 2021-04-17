
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<signal.h>
#include <time.h>
#include <string.h>
#include "worker.h"

clock_t tiempo_inicio, tiempo_final;
int interrumped, status, process_number,n;
pid_t child_pid;
FILE* file;
char* buffer;
char** args;
char dest[100] = "";

void handle_sigint()
{
  signal(SIGINT, handle_sigint);
  printf("\n--Señal SIGINT recibida--\n");
}

void handle_sigabort()
{
  signal(SIGABRT, handle_sigabort);
  tiempo_final = clock();
  printf("\n--Señal SIGABRT recibida--\n");
  interrumped = 1;
  final_output_format();
}

void output_format(char* dest, int n, char** args){
	for (int i = 0; i < n + 1; i++)
	{
		strcat(dest, args[i]);
		if (i < n)
		{
		    strcat(dest, ",");
		}
	}
}

void final_output_format()
{
  output_format(dest, n, args);
  sprintf(buffer,"./%i.txt",process_number);
  file = fopen(buffer,"w");
  sprintf(buffer, "%s,%f,%i,%i\n",dest,(double)(tiempo_final - tiempo_inicio)/CLOCKS_PER_SEC,status, interrumped);
  fputs(buffer,file);
  fclose(file);
}

void worker(char** process, int number)
{
  process_number = number;
  interrumped = 0;
  buffer = malloc(100*sizeof(char));
  n = atoi(process[2]);
  args = malloc( (n + 2) * sizeof(char*)); /* Se pide memoria para la cantida de procesos mas el archivo  y el puntero final null*/
  args[0] = process[1];
  for (int i = 0; i < n; i++)
  {
    args[ i + 1] = process[3 + i];
  }
  args[ n + 1] = NULL;
  tiempo_inicio = clock();
  if ((child_pid = fork()) != 0)
  {
    signal(SIGINT,handle_sigint);
    signal(SIGABRT, handle_sigabort);
    waitpid(child_pid,&status,0);
    if (!interrumped)
    {
      tiempo_final = clock();
      final_output_format();
      
    }
    free(buffer);
    free(args);
  } 
  else 
  {
    execve(args[0], args, 0);
  }
}
