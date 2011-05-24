#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "des.h"

// Everything is big endian here... (0 = MSB)


unsigned char ip_table[] = {58, 50, 42, 34, 26, 18, 10,  2,
			    60, 52, 44, 36, 28, 20, 12,  4,
			    62, 54, 46, 38, 30, 22, 14,  6,
			    64, 56, 48, 40, 32, 24, 16,  8,
			    57, 49, 41, 33, 25, 17,  9,  1,
			    59, 51, 43, 35, 27, 19, 11,  3,
			    61, 53, 45, 37, 29, 21, 13,  5,
			    63, 55, 47, 39, 31, 23, 15,  7};

unsigned char fp_table[] = {40,  8, 48, 16, 56, 24, 64, 32,
			    39,  7, 47, 15, 55, 23, 63, 31,
			    38,  6, 46, 14, 54, 22, 62, 30,
			    37,  5, 45, 13, 53, 21, 61, 29,
			    36,  4, 44, 12, 52, 20, 60, 28,
			    35,  3, 43, 11, 51, 19, 59, 27,
			    34,  2, 42, 10, 50, 18, 58, 26,
			    33,  1, 41,  9, 49, 17, 57, 25};


unsigned char e_table[] = { 32, 1, 2, 3, 4, 5,
			    4, 5, 6, 7, 8, 9,
			    8, 9, 10, 11, 12, 13,
			    12, 13, 14, 15, 16, 17,
			    16, 17, 18, 19, 20, 21,
			    20, 21, 22, 23, 24, 25,
			    24, 25, 26, 27, 28, 29,
			    28, 29, 30, 31, 32, 1};


unsigned char s1_table[] = { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
			      0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
			      4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
			     15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13};

unsigned char s2_table[] = { 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
			      3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
			      0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
			     13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

unsigned char s3_table[] = { 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
			     13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
			     13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
			      1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

unsigned char s4_table[] = {  7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
			     13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
			     10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
			      3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

unsigned char s5_table[] = {  2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
			     14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
			      4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
			     11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

unsigned char s6_table[] = { 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
			     10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
			      9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
			      4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};

unsigned char s7_table[] = {  4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
			     13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
			      1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
			      6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

unsigned char s8_table[] = { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
			      1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
			      7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
			      2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11};


unsigned char p_table[] = {16,  7, 20, 21, 29, 12, 28, 17,
			    1, 15, 23, 26,  5, 18, 31, 10,
			    2,  8, 24, 14, 32, 27,  3,  9,
			   19, 13, 30,  6, 22, 11,  4, 25};


unsigned char p_inv_table[] = { 9, 17, 23, 31, 13, 28,  2, 18,
				24, 16, 30,  6, 26, 20, 10,  1,
				8, 14, 25,  3,  4, 29, 11, 19,
				32, 12, 22,  7,  5, 27, 15, 21};


unsigned char pc1_table[] = {57, 49, 41, 33, 25, 17,  9,
			      1, 58, 50, 42, 34, 26, 18,
			     10,  2, 59, 51, 43, 35, 27,
			     19, 11,  3, 60, 52, 44, 36,
			     63, 55, 47, 39, 31, 23, 15,
			      7, 62, 54, 46, 38, 30, 22,
			     14,  6, 61, 53, 45, 37, 29,
			     21, 13,  5, 28, 20, 12,  4};

unsigned char pc2_table[] = {14, 17, 11, 24,  1,  5,
			      3, 28, 15,  6, 21, 10,
			     23, 19, 12,  4, 26,  8,
			     16,  7, 27, 20, 13,  2,
			     41, 52, 31, 37, 47, 55,
			     30, 40, 51, 45, 33, 48,
			     44, 49, 39, 56, 34, 53,
			     46, 42, 50, 36, 29, 32};


unsigned char subkeys[16 * 6];



unsigned char get_bit(unsigned char * data,
		      int bit_num) {

  int byte_in_data = bit_num / 8;
  int bit_in_byte = bit_num - byte_in_data * 8;

  unsigned char mask = (1 << (7 - bit_in_byte));

  unsigned result = data[byte_in_data] & mask;

  return result;
}


void set_bit(unsigned char * data,
	     int bit_num,
	     unsigned char value) {

  int byte_in_data = bit_num / 8;
  int bit_in_byte = bit_num - byte_in_data * 8;

  unsigned char mask = (1 << (7 - bit_in_byte));

  if (value == 0)
    data[byte_in_data] &= (~mask);
  else
    data[byte_in_data] |= mask;
}


void apply_s_box(unsigned char * block_in,
		 int start_bit_in,
		 unsigned char * block_out,
		 int start_bit_out,
		 unsigned char * table) {

  unsigned char row = 0;
  unsigned char column = 0;

  unsigned char box_output;

  set_bit(&row, 6, get_bit(block_in, start_bit_in));
  set_bit(&row, 7, get_bit(block_in, start_bit_in + 5));

  set_bit(&column, 4, get_bit(block_in, start_bit_in + 1));
  set_bit(&column, 5, get_bit(block_in, start_bit_in + 2));
  set_bit(&column, 6, get_bit(block_in, start_bit_in + 3));
  set_bit(&column, 7, get_bit(block_in, start_bit_in + 4));

  box_output = table[row * 16 + column];

  set_bit(block_out, start_bit_out, get_bit(&box_output, 4));
  set_bit(block_out, start_bit_out + 1, get_bit(&box_output, 5));
  set_bit(block_out, start_bit_out + 2, get_bit(&box_output, 6));
  set_bit(block_out, start_bit_out + 3, get_bit(&box_output, 7));
}


void des_feistel(unsigned char * block_in,
		 unsigned char * block_out,
		 unsigned char * subkey) {

  int i;

  unsigned char block_extended[6];
  unsigned char block_with_key[6];
  unsigned char block_after_s[4];

  memset(block_extended, 0, 6);
  memset(block_after_s, 0, 4);
  memset(block_out, 0, 4);


  // Expansion function (E)
  for (i = 0; i < 48; i++)
    set_bit(block_extended, i, get_bit(block_in, e_table[i] - 1));


  // XOR with the subkey
  for (i = 0; i < 6; i++)
    block_with_key[i] = block_extended[i] ^ subkey[i];

  // S-boxes
  apply_s_box(block_with_key,  0, block_after_s,  0, s1_table);
  apply_s_box(block_with_key,  6, block_after_s,  4, s2_table);
  apply_s_box(block_with_key, 12, block_after_s,  8, s3_table);
  apply_s_box(block_with_key, 18, block_after_s, 12, s4_table);
  apply_s_box(block_with_key, 24, block_after_s, 16, s5_table);
  apply_s_box(block_with_key, 30, block_after_s, 20, s6_table);
  apply_s_box(block_with_key, 36, block_after_s, 24, s7_table);
  apply_s_box(block_with_key, 42, block_after_s, 28, s8_table);

  // Permutation (P)
  for (i = 0; i < 32; i++)
    set_bit(block_out, i, get_bit(block_after_s, p_table[i] - 1));
}


void des_round(unsigned char * block_in,
	       unsigned char * block_out,
	       unsigned char * subkey) {

  int i;

  unsigned char * left_in = block_in;
  unsigned char * right_in = &(block_in[4]);

  unsigned char * left_out = block_out;
  unsigned char * right_out = &(block_out[4]);

  unsigned char feistel_result[4];

  des_feistel(right_in, feistel_result, subkey);

  for (i = 0; i < 4; i++)
    right_out[i] = feistel_result[i] ^ left_in[i]; // R_(i+1) = L_i ^ f(R_i, K_i)

  memcpy(left_out, right_in, 4); // L_(i+1) = R_i

}


void key_register_rotate(unsigned char *key_register) {
  int i;

  unsigned char bit0;

  // Left part
  bit0 = get_bit(key_register, 0);

  for (i = 0; i < 27; i++)
    set_bit(key_register, i, get_bit(key_register, i + 1));

  set_bit(key_register, 27, bit0);

  // Right part
  bit0 = get_bit(key_register, 28);

  for (i = 28; i < 55; i++)
    set_bit(key_register, i, get_bit(key_register, i + 1));

  set_bit(key_register, 55, bit0);
}


void compute_subkeys(unsigned char *key) {
  int i;

  unsigned char key_register[7];

  memset(key_register, 0, 7);

  // PC1
  for (i = 0; i < 56; i++)
    set_bit(key_register, i, get_bit(key, pc1_table[i] - 1));

  // Round 1
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(subkeys, i, get_bit(key_register, pc2_table[i] - 1));

  // Round 2
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[1*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 3
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[2*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 4
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[3*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 5
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[4*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 6
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[5*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 7
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[6*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 8
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[7*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 9
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[8*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 10
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[9*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 11
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[10*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 12
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[11*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 13
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[12*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 14
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[13*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 15
  key_register_rotate(key_register);
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[14*6], i, get_bit(key_register, pc2_table[i] - 1));

  // Round 16
  key_register_rotate(key_register);
  for (i = 0; i < 48; i++)
    set_bit(&subkeys[15*6], i, get_bit(key_register, pc2_table[i] - 1));
}


void des_encrypt(unsigned char *block_in,
		 unsigned char *block_out,
		 unsigned char *key) {

  int i;

  unsigned char in_after_ip[8];
  unsigned char intermediate_block[8];
  unsigned char intermediate_block2[8];

  memset(in_after_ip, 0, 8);
  memset(intermediate_block, 0, 8);
  memset(intermediate_block2, 0, 8);


  // Compute subkeys
  compute_subkeys(key);


  // Initial Permutation
  for (i = 0; i < 64; i++)
    set_bit(in_after_ip, i, get_bit(block_in, ip_table[i] - 1));

  // Rounds
  des_round(in_after_ip, intermediate_block, &subkeys[0]); // 1
  des_round(intermediate_block, intermediate_block2, &subkeys[1 * 6]); //  2
  des_round(intermediate_block2, intermediate_block, &subkeys[2 * 6]); //  3
  des_round(intermediate_block, intermediate_block2, &subkeys[3 * 6]); //  4
  des_round(intermediate_block2, intermediate_block, &subkeys[4 * 6]); //  5
  des_round(intermediate_block, intermediate_block2, &subkeys[5 * 6]); //  6
  des_round(intermediate_block2, intermediate_block, &subkeys[6 * 6]); //  7
  des_round(intermediate_block, intermediate_block2, &subkeys[7 * 6]); //  8
  des_round(intermediate_block2, intermediate_block, &subkeys[8 * 6]); //  9
  des_round(intermediate_block, intermediate_block2, &subkeys[9 * 6]); // 10
  des_round(intermediate_block2, intermediate_block, &subkeys[10 * 6]); // 11
  des_round(intermediate_block, intermediate_block2, &subkeys[11 * 6]); // 12
  des_round(intermediate_block2, intermediate_block, &subkeys[12 * 6]); // 13
  des_round(intermediate_block, intermediate_block2, &subkeys[13 * 6]); // 14
  des_round(intermediate_block2, intermediate_block, &subkeys[14 * 6]); // 15
  des_round(intermediate_block, intermediate_block2, &subkeys[15 * 6]); // 16

  // Undo the final swap
  memcpy(intermediate_block, intermediate_block2, 8);
  memcpy(intermediate_block2, &(intermediate_block2[4]), 4);
  memcpy(&(intermediate_block2[4]), intermediate_block, 4);

  // Final Permutation
  for (i = 0; i < 64; i++)
    set_bit(block_out, i, get_bit(intermediate_block2, fp_table[i] - 1));
}



void get_subkey(int num_subkey, unsigned char *key, unsigned char *subkey_out) {
  // Compute subkeys
  compute_subkeys(key);

  memcpy(subkey_out, &subkeys[num_subkey], 6);
}
