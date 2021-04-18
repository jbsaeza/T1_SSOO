#pragma once

#include "../file_manager/manager.h"


void run_process();
void run_worker(char *file, char* n_args, char** args);
void run_manager(InputFile* input_file, char* time_out, char* n_lines, char** line, bool root);