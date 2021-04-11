#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../file_manager/manager.h"

/* 
El proceso worker recibe un ejecutable, el numero de argumentos y luego los argumentos

Ejecuta el programa que recibe como input y lo espera, luego retorna estadisticas en un archivo
Debe interceptar señales sigint y sigabort (la primera se ignora y la segunda es de interrupcion,
indica que el proceso padre debe escribir las estadisticas antes de que el hijo termine)


*/

// void worker(char *file, int n, char *args[])
// {
//   if (fork() != 0)
//   {
//     waitpid(-1, &status, 0);
//   } 
//   else 
//   {
//     execve(file, parameters, 0);
//   } 
// }


int main(int argc, char **argv)
{
  // FILE* fp;
  // int file_length;
  // char id;
  // char* rest_line;
  // int n;
  // char* arguments;
  // char* executable;
  // char* input  = argv[1];
  // int process = atoi(argv[2]);
  // fp = fopen(input, "r");
  // if (fp==NULL) {fputs ("File error",stderr); exit (1);}
  // fscanf(fp, "%d\n", &file_length);
  // for (int i = process; i < file_length; i++)
  // {
  //   fscanf(fp,"%c,%s\n",&id,rest_line);
  //   printf("Leyendo proceso tipo %c con resto de linea %s\n",id,rest_line);
  //   if (id == 'W') {
  //     printf("Se ejecuta un worker\n");
  //     /*
  //     printf("Ejecutable: %s\n",executable);
  //     printf("Numero de argumentos: %i\n",n);
  //     printf("Argumentos: %s\n",arguments);
  //     */
  //   }
  //   else
  //   {
  //     printf("Se ejecuta un manager\n");
  //   }
  // }
  // fclose(fp);
  InputFile* input_file = read_file(argv[1]);
  int process = atoi(argv[2]);
  printf("Empezando\n");
  for (int i = 0; i < input_file -> len; i++)
  {
    for (int j = 0; j < (sizeof(input_file -> lines[i])/sizeof(input_file -> lines[i][0])); j++)
    {
      printf("%s\n",input_file -> lines[i][j]);
    }
  }
  printf("Terminando\n");
  input_file_destroy(input_file);
  return 0;
}
