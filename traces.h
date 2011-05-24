#ifndef TRACES_H
#define TRACES_H

#include <stdint.h>

typedef struct {
  uint8_t message[8];
  uint8_t key[8];
  uint8_t ciphertext[8];
  int num_samples;
  float *samples;
} trace_s;


typedef struct trace_node_s {
  trace_s *trace;
  struct trace_node_s *next;
} trace_node;


extern trace_node *traces_list;


void read_traces_from_dir(char * directory);


#endif /* TRACES_H */
