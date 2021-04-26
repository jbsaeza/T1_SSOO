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
      worker(input_file -> lines[process], process);
      break;

    case 'M':
      run_manager(input_file, input_file -> lines[process][1], input_file -> lines[process][2], input_file -> lines[process], false, process);
      break;

    case 'R':
      run_manager(input_file, input_file -> lines[process][1], input_file -> lines[process][2], input_file -> lines[process], true, process);
      break;
  }
  input_file_destroy(input_file);
  return 0;  
}
