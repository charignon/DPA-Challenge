#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "dpa.h"
#include "traces.h"
#include "des.h"
#include "display.h"
#include "hamming.h"



//Functions to handle datapath through the system
void destination_index (int s_box , unsigned int*  destination_indexes ) ;
void source_index (unsigned int*  source_indexes , unsigned int *destination_indexes);
void s_box (int num_sbox , unsigned int*  source_indexes , unsigned int *destination_indexes);



unsigned char inv_ip_table_2[] =
  {
    40, 8, 48, 16, 56, 24, 64, 32, 39,
    7, 47, 15, 55, 23, 63, 31, 38, 6, 
    46, 14, 54, 22, 62, 30, 37, 5, 45, 
    13, 53, 21, 61, 29, 36, 4, 44, 12, 
    52, 20, 60, 28, 35, 3, 43, 11, 51, 
    19, 59, 27, 34, 2, 42, 10, 50, 18, 
    58, 26, 33, 1, 41, 9, 49, 17, 57, 25
  };





//Get the indexes of the bits sent through a s_box for calculation
void init_index( int num_sbox , unsigned int * initial_indexes)
{

unsigned char e_table[] = { 32, 1, 2, 3, 4, 5,
			    4, 5, 6, 7, 8, 9,
			    8, 9, 10, 11, 12, 13,
			    12, 13, 14, 15, 16, 17,
			    16, 17, 18, 19, 20, 21,
			    20, 21, 22, 23, 24, 25,
			    24, 25, 26, 27, 28, 29,
			    28, 29, 30, 31, 32, 1};

  
  for (int i = 0 ; i < 6 ; i ++)
    {
      initial_indexes[i] = inv_ip_table_2[e_table[i]-1]-1;
    }

}




int compute_sbox ( int num_sbox , trace_s* t , unsigned int * initial_indexes , unsigned char sub_key )
{
  
  unsigned char input=0x00;
  unsigned char before_sbox = 0x00;
  unsigned char after_sbox;
  
   
     
   unsigned char s1_table[] = { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
				0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
				4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
				15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13};
  

  for (int i = 0 ; i < 6 ; i ++ )
    {
      input|= get_bit_new(t->message,initial_indexes[i])<<i;
    }
  
  before_sbox = input^sub_key;
  after_sbox = s1_table[before_sbox];

  return after_sbox;
  
}






void  destination_index (int s_box , unsigned int*  destination_indexes ) 
{

unsigned char p_table[] = {16,  7, 20, 21, 29, 12, 28, 17,
			    1, 15, 23, 26,  5, 18, 31, 10,
			    2,  8, 24, 14, 32, 27,  3,  9,
			   19, 13, 30,  6, 22, 11,  4, 25};

  for (int i = 0 ; i < 4 ; i ++ )
    {
      destination_indexes[i] = p_table[4*(s_box-1) + i ] -1 ;
    }
}

//To be used with the result of the previously mentionned function : destination_index
void source_index (unsigned int*  source_indexes , unsigned int *destination_indexes)
{
  for (int i = 0 ; i < 4 ; i ++ ) 
    source_indexes[i] = inv_ip_table_2[destination_indexes[i]] - 1 ;
   
}


void s_box (int num_sbox , unsigned int*  source_indexes , unsigned int *destination_indexes)
{
  destination_index(num_sbox, destination_indexes );
  source_index(source_indexes , destination_indexes);
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
  /* trace_node* current; */
  /* current = traces_list; */
  /* unsigned char * out = (unsigned char*) malloc(sizeof(unsigned char)*48); */
  /* for (int i = 0 ; i <atoi(argv[2]) ; i ++) */
  /*   { */
  /*     printf("\nTRACE NODE NUMBER %d :",i); */
  /*     printf("\n  message : "); */
  /*     dump_string_hexa(8,current->trace->message); */
  /*     printf("\n%f\n",average_sample(current->trace));   */
  /*     current = current->next; */
  /*   } */
    
   unsigned int * result_s1 = (unsigned int * ) malloc ( sizeof(unsigned int) * 4 ); 
   unsigned int * result_clair = (unsigned int * ) malloc ( sizeof(unsigned int) * 4 );  
   unsigned int * init = (unsigned int * ) malloc ( sizeof(unsigned int) * 4 );
   int after_sbox ;
   int dist ;
   
   
   
   s_box(1,result_clair, result_s1); 
   init_index(1,init);
   
   trace_node * current;
   current = traces_list;
   
   for (int i = 0 ; i < atoi(argv[2]);i++)
     {
       
       after_sbox = compute_sbox ( 1,current->trace ,init , (unsigned char) 0x1F );
       dist = hamming_dpa(current->trace , after_sbox , result_clair);
       current = current->next;
       
     }
   
  
   free(result_s1);
   free(result_clair);
   free(init);

  

  //  free(out);
  exit (EXIT_SUCCESS);
}
