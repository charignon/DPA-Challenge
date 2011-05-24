#ifndef DES_H
#define DES_H

unsigned char get_bit(unsigned char * data, int bit_num);
void set_bit(unsigned char * data, int bit_num, unsigned char value);

void apply_s_box(unsigned char * block_in,
		 int start_bit_in,
		 unsigned char * block_out,
		 int start_bit_out,
		 unsigned char * table);

void des_feistel(unsigned char * block_in,
		 unsigned char * block_out,
		 unsigned char * subkey);

void des_round(unsigned char * block_in,
	       unsigned char * block_out,
	       unsigned char * subkey);

void key_register_rotate(unsigned char *key_register);

void compute_subkeys(unsigned char *key);

void des_encrypt(unsigned char *block_in,
		 unsigned char *block_out,
		 unsigned char *key);

void get_subkey(int num_subkey, unsigned char *key, unsigned char *subkey_out);




#endif /*  DES_H */
