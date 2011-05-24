#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "dpa.h"
#include "traces.h"
#include "des.h"


//Display functions
void dump_byte(unsigned char a);
void dump_trace_node(trace_s *a);
void show_des_result(unsigned char * message, unsigned char * out, unsigned char * key);
void dump_array(char * file_name , int length, float* array);

//Hamming distances functions
int hamming_string(int length, unsigned char * s1,unsigned char * s2);
int hamming_byte(unsigned char byte1, unsigned char byte2);
int hamming_bit ( int length, unsigned char c1, unsigned char c2);



double average_sample(trace_s *a)
{
  double total = 0;
  for (int i = 0 ; i < a->num_samples ; i ++)
    total += a->samples[i];
  total = total / a->num_samples;
  return total;
}

void dump_array(char * file_name , int length, float* array)
{
  FILE * f = fopen(file_name,"w");
  for (int i =0 ; i < length ; i ++)
    {
      fprintf(f,"%f\n",array[i]);
    }
  fclose(f);
}



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
{  for (int j = 8 ; j!= 0 ; j --)
    printf("%d",((a)&(0x1<<j))>>j);
}

void dump_string(int length, unsigned char * st)
{  for (int l = 0 ; l < length ; l ++)
    dump_byte(st[l]);
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






int main(int argc, char **argv)
{
  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s trace_directory\n", argv[0]);
      exit (EXIT_FAILURE);
    }


  /**************************
   *    SAMPLES ACQUIRING   *
   **************************/
  printf("Reading traces from disk... ");
  fflush(stdout);
  read_traces_from_dir(argv[1]);
 

  /**********************
   *     PROCESSING     *
   **********************/
  trace_node* current;
  current = traces_list;
  unsigned char * out = (unsigned char*) malloc(sizeof(unsigned char)*48);
  for (int i = 0 ; i <atoi(argv[2]) ; i ++)
    {
      printf("\nTRACE NODE NUMBER %d :",i);
      printf("\n  message : ");
      dump_string_hexa(8,current->trace->message);
      printf("\n%f\n",average_sample(current->trace));  
      current = current->next;
    }
    
  

  free(out);
  exit (EXIT_SUCCESS);
}
