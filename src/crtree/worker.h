#pragma once

void handle_sigabort();
void handle_sigint();
void worker(char** process, int number);
void final_output_format();
void output_format(char* dest, int n, char** args);
