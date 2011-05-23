#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "dpa.h"
#include "traces.h"
#include "des.h"


void dump_byte(unsigned char a);
void dump_trace_node(trace_s *a);
void show_des_result(unsigned char * message, unsigned char * out, unsigned char * key);


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
{
       for (int j = 8 ; j!= 0 ; j --)
	 printf("%d",((a)&(0x1<<j))>>j);
  
}


void show_des_result(unsigned char * message, unsigned char * out, unsigned char * key)
{
      des_encrypt(message,out ,key);
      printf("\n RESULT :\n ");
      for (int l=0;l<8;l++)
	dump_byte(out[l]);
}


int main(int argc, char **argv)
{
  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s trace_directory\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  printf("Reading traces from disk... ");
  fflush(stdout);

  read_traces_from_dir(argv[1]);
 trace_node* current;
  current = traces_list;
  unsigned char * out = (unsigned char*) malloc(sizeof(unsigned char)*48);
  for (int i = 0 ; i <atoi(argv[2]) ; i ++)
    {
      printf("TRACE NODE NUMBER %d :\n",i); 
      dump_trace_node(current->trace);
      show_des_result(current->trace->message,out ,current->trace->key);
      current = current->next;
      }
 
      
  free(out);
  
  printf("done.\n");

  // TODO

  exit (EXIT_SUCCESS);
}