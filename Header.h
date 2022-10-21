#pragma once
#pragma once
int read_data(const char* filename, char data[10][10][1000], char macro[10][1000], int* macro_count);

bool read_input(const char* filename, char* input);

void replace_macro(char* input_text, char macro[10][1000], int macro_count, char data[10][10][1000], int data_count, char output_text[10][1000]);
