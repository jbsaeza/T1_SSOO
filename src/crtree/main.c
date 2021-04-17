#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../file_manager/manager.h"
#include "worker.h"

/* Interrupt the process */ 
#define SIGINT  2 

/* Abort. */
#define SIGABRT 6

void handle_signal(int sig)
{
  printf("Caught signal %d\n", sig);
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
