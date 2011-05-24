#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "hamming.h"

unsigned char inv_ip_table[] =
  {
    40, 8, 48, 16, 56, 24, 64, 32, 39,
    7, 47, 15, 55, 23, 63, 31, 38, 6, 
    46, 14, 54, 22, 62, 30, 37, 5, 45, 
    13, 53, 21, 61, 29, 36, 4, 44, 12, 
    52, 20, 60, 28, 35, 3, 43, 11, 51, 
    19, 59, 27, 34, 2, 42, 10, 50, 18, 
    58, 26, 33, 1, 41, 9, 49, 17, 57, 25
  };


int hamming_dpa (trace_s *t ,  int sbox_out , unsigned int * destination_index )
{
  unsigned char initial_bits = 0;
  for (int i = 0 ; i < 4 ; i ++)
    initial_bits |= get_bit_new(t->message,inv_ip_table[destination_index[i]])<<i ;
  return hamming_byte(initial_bits,sbox_out);
  
  
  
}



int hamming_byte(unsigned char byte1, unsigned char byte2)
{
  return hamming_bit(8,byte1,byte2);
}

int hamming_string(int length, unsigned char * s1,unsigned char * s2)
{
  int count = 0 ;
  for (int l = 0 ; l < length ; l ++)
    count += hamming_byte(s1[l],s2[l]);
  return count;
 
}

int hamming_bit ( int length, unsigned char c1, unsigned char c2)
{
  //Compute the Hamming distance between the length first bits of the given bytes c1 and c2
  int count = 0 ;
  for (int i = 0 ; i < length ; i++)
    if ((c1&(0x1<<i)) != (c2&(0x1<<i)))
      count ++;
  return count ;
}

unsigned char get_bit_new(unsigned char * data, int bit_num) {



  int byte_in_data = bit_num / 8;
  int bit_in_byte = bit_num - byte_in_data * 8;

  unsigned char mask = (1 << (7 - bit_in_byte));

  unsigned result = (data[byte_in_data] & mask ) >>(7 - bit_in_byte);

  return result;
}

