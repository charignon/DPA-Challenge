#ifndef HAMMING_H
#define HAMMING_H
//Hamming distances function
#include "traces.h"
int hamming_string(int length, unsigned char * s1,unsigned char * s2);
int hamming_byte(unsigned char byte1, unsigned char byte2);
int hamming_bit ( int length, unsigned char c1, unsigned char c2);
int hamming_dpa (trace_s *t ,  int sbox_out , unsigned int * destination_index );
unsigned char get_bit_new(unsigned char * data, int bit_num) ;



#endif
