#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "../file_manager/manager.h"
#include "manager.h"
#include "worker.h"

int main(int argc, char **argv)
{
  InputFile* input_file = read_file(argv[1]);
  int process = atoi(argv[2]);
  switch (*input_file -> lines[process][0])
  {
    case 'W':
      printf("Ejecutando Worker: %s\n", input_file -> lines[process][1]);
      worker(input_file -> lines[process], process);
      break;

    case 'M':
      printf("Ejecutando Manager\n");
      run_manager(input_file, input_file -> lines[process][1], input_file -> lines[process][2], input_file -> lines[process], false, process);
      break;

    case 'R':
      printf("Ejecutando Root\n");
      run_manager(input_file, input_file -> lines[process][1], input_file -> lines[process][2], input_file -> lines[process], true, process);
      break;
  }
  return 0;  
}
