#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "worker.h"

time_t tiempo_inicio, tiempo_final;
int interrumped, status, process_number,n;
pid_t child_pid;
FILE* file;
char* buffer;
char** args;
char* dest;

char* stringRemoveNonAlphaNum(char* str)
{
  unsigned long i = 0;
  unsigned long j = 0;
  char c;
  while ((c = str[i++]) != '\0')
  {
      if (isalnum(c))
      {
          str[j++] = c;
      }
  }
  str[j] = '\0';
  return str;
}

void handle_sigabort()
{
  interrumped = 1;
  tiempo_final = time(NULL);
  final_output_format();
  waitpid(child_pid,&status,0);
  free(buffer);
  free(args);
  return;
}

void output_format(){
  dest = malloc(sizeof(char)*255);
  strcpy(dest, "");
	for (int i = 0; i <= n; i++)
	{
    if (i == 0)
    {
      strcat(dest, args[i]);
      strcat(dest, ",");
    }
    else
    {
      strcat(dest, stringRemoveNonAlphaNum(args[i]));
      strcat(dest, ",");
    }
	}
}

void final_output_format()
{
  output_format();
  sprintf(buffer,"./%i.txt",process_number);
  file = fopen(buffer,"w");
  sprintf(buffer, "%s%d,%ld,%i,%i\n",dest, n, (tiempo_final - tiempo_inicio), status, interrumped);
  fputs(buffer,file);
  fclose(file);
  free(dest);
}

void worker(char** process, int number)
{
  process_number = number;
  interrumped = 0;
  buffer = malloc(100);
  n = atoi(process[2]);
  args = malloc( (n + 2) * sizeof(char*)); /* Se pide memoria para la cantida de procesos mas el archivo  y el puntero final null*/
  args[0] = process[1];
  for (int i = 0; i < n; i++)
  {
    args[ i + 1] = process[3 + i];
  }
  args[ n + 1] = NULL;
  tiempo_inicio = time(NULL);
  if ((child_pid = fork()) != 0)
  {
    signal(SIGINT, SIG_IGN);
    signal(SIGABRT, handle_sigabort);
    waitpid(child_pid,&status,0);
    tiempo_final = time(NULL);
    final_output_format();
    free(buffer);
    free(args);
    return;
  } 
  else 
  {
    execve(args[0], args, 0);
    free(buffer);
    free(args);
    return;
  }
}
