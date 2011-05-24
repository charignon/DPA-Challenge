#ifndef DISPLAY_H
#define DISPLAY_H
#include "traces.h"

//Display functions
void dump_byte(unsigned char a);
void dump_trace_node(trace_s *a);
void show_des_result(unsigned char * message, unsigned char * out, unsigned char * key);
void dump_array(char * file_name , int length, float* array);
void dump_string(int length, unsigned char * st);
void dump_string_hexa(int length, unsigned char * st);

#endif
