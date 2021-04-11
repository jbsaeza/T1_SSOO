#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../file_manager/manager.h"

int main(int argc, char **argv)
{
  InputFile* input_file = read_file(argv[1]);
  int process = atoi(argv[2]);
  for (int i = process; i < input_file -> len; i++) {
    switch (input_file -> lines[i][0][0])
    {
    case 'W':
      printf("Ejecutando Worker\n");
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
