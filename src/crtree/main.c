#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "../file_manager/manager.h"
#include "manager.h"

pid_t parent = 1;
int main(int argc, char **argv)
{
  InputFile* input_file = read_file(argv[1]);
  int process = atoi(argv[2]);
  for (int i = process; i < input_file -> len && parent != 0; i++) {
    parent = fork();
    if (parent == 0)
    {
      printf("_________________________________________\n");
      switch (*input_file -> lines[i][0])
      {
        case 'W':
          printf("Ejecutando Worker: %s\n", input_file -> lines[i][1]);
          break;

        case 'M':
          printf("Ejecutando Manager\n");
          run_manager(input_file, input_file -> lines[i][1], input_file -> lines[i][2], input_file -> lines[i], false);
          break;

        case 'R':
          printf("Ejecutando Root\n");
          run_manager(input_file, input_file -> lines[i][1], input_file -> lines[i][2], input_file -> lines[i], true);
          break;
      }
    }
    
  }
  if (parent != 0){
    input_file_destroy(input_file);
    return 0;
  }
  
}
