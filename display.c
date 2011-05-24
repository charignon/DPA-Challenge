#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "display.h"
#include "des.h"

void dump_trace_node(trace_s *a)
{
  for (int k =0;k<8;k++)
    {
      printf("PLAIN TEXT : \n");
      dump_byte(a->message[k]);
      printf("\nCIPHERED :\n");
      dump_byte(a->ciphertext[k]);
      printf("\nKEY :\n");
      dump_byte(a->key[k]);
    }
}

void dump_byte(unsigned char a)
{  for (int j = 7 ; j!=-1 ; j --)
    printf("%d",((a)&(0x1<<j))>>j);
}

void dump_string(int length, unsigned char * st)
{  for (int l = 0 ; l < length ; l ++)
    {  dump_byte(st[l]);printf(" ");
    }
  
}

void dump_string_hexa(int length, unsigned char * st)
{    for (int l = 0 ; l < length ; l ++)
      printf("%02x",st[l]);
}


void show_des_result(unsigned char * message, unsigned char * out, unsigned char * key)
{  des_encrypt(message,out ,key);
  printf("\n RESULT :\n ");
  for (int l=0;l<8;l++)
    dump_byte(out[l]);}

void dump_array(char * file_name , int length, float* array)
{
  FILE * f = fopen(file_name,"w");
  for (int i =0 ; i < length ; i ++)
    {
      fprintf(f,"%f\n",array[i]);
    }
  fclose(f);
}
