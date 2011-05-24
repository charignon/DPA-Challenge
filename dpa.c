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


//Functions to handle datapath through the system
void destination_index (int s_box , unsigned int*  destination_indexes ) ;
void source_index (unsigned int*  source_indexes , unsigned int *destination_indexes);
void s_box (int num_sbox , unsigned int*  source_indexes , unsigned int *destination_indexes);









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
      initial_indexes[i] = inv_ip_table[e_table[i]-1]-1;
    }

}


void compute_sbox ( int num_sbox , trace_s* t , unsigned int * initial_indexes , unsigned char sub_key , unsigned int * sbox_output)
{
  
  unsigned char input=0;
   int j ;
   unsigned char bit_to_add ;
   unsigned char found_char;
   
   int char_num ;
   int pos_num ;
     
  
  for (int i = 0 ; i < 6 ; i ++ )
    {
      j = initial_indexes[i];
      bit_to_add = 0;
      char_num = j/8;
      pos_num = j%8;
      found_char = t->message[char_num];
      bit_to_add = (found_char & (0x1 << pos_num) ) >>pos_num;
      input|= bit_to_add<<i;
           
    }
  
  dump_byte(input^sub_key);
  
  

}



























  





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
    source_indexes[i] = inv_ip_table[destination_indexes[i]] - 1 ;
   
}


void s_box (int num_sbox , unsigned int*  source_indexes , unsigned int *destination_indexes)
{
  destination_index(num_sbox, destination_indexes );
  source_index(source_indexes , destination_indexes);
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
    
  /* unsigned int * result_s1 = (unsigned int * ) malloc ( sizeof(unsigned int) * 4 ); */
  /* unsigned int * result_clair = (unsigned int * ) malloc ( sizeof(unsigned int) * 4 ); */
  
  
  /* s_box(1,result_clair, result_s1); */
  
  /* for (int k = 0 ; k < 4 ; k ++ )  */
  /*   printf("%d\n",result_clair[k]); */
  

  unsigned int * init = (unsigned int * ) malloc ( sizeof(unsigned int) * 4 );
  init_index(1,init);
  for (int k = 0 ; k < 6 ; k ++ ) 
    printf("%d\n",init[k]);
  
  

  //void compute_sbox ( 1 , trace_node t , int ,0xFF , null)


  //  free(result_s1);
  //free(result_clair);
  free(init);

  

  //  free(out);
  exit (EXIT_SUCCESS);
}
