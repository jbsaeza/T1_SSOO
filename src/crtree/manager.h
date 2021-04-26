#pragma once

#include "../file_manager/manager.h"


void run_process();
void run_manager(InputFile* input_file, char* time_out, char* n_lines, char** line, bool root, int index);
void generate_output();
void free_worker();
void free_manager();