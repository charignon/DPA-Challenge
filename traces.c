#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <dirent.h>
#include <unistd.h>

#include "traces.h"
#include "des.h"


trace_node *traces_list = NULL;
trace_node *latest_trace = NULL;


void verify_des(trace_s *trace) {
  unsigned char output[8];
  int result;

  des_encrypt(trace->message, output, trace->key);

  result = memcmp(output, trace->ciphertext, 8);

  assert(result == 0);
}


void add_trace_to_list(trace_s *trace) {
  trace_node *node;

  if (traces_list == NULL) {
    // New list
    traces_list = (trace_node *) malloc(sizeof(trace_s));
    traces_list->trace = trace;
    traces_list->next = NULL;

    latest_trace = traces_list;
  }
  else {
    // Append to the existing list
    node = (trace_node *) malloc(sizeof(trace_s));
    node->trace = trace;
    node->next = NULL;

    latest_trace->next = node;
    latest_trace = node;
  }
}


void read_trace(char *filename, trace_s *trace) {
  int fd;
  int size;

  fd = open(filename, O_RDONLY);

  if (fd < 0) {
    perror("read_trace");
    trace->num_samples = 0;
    return;
  }

  // Skip file header
  lseek(fd, 12, SEEK_CUR);

  // Skip waveform header
  read(fd, &size, 4);
  lseek(fd, size - 4, SEEK_CUR);

  // Skip (a part of) the data header
  read(fd, &size, 4);
  lseek(fd, size - 8, SEEK_CUR);

  // Read the data
  read(fd, &size, 4);
  trace->num_samples = size / 4;

  assert(size > 0);
  assert(size == (trace->num_samples * 4));

  trace->samples = (float *) malloc(size);
  read(fd, trace->samples, size);

  close(fd);
}


void read_traces_from_dir(char * directory) {
  int res;
  DIR *traces_dir;
  struct dirent *directory_entry;
  struct stat file_stats;

  char *key, *message, *ciphertext;

  trace_s *trace;


  res = chdir(directory);
  if (res < 0) {
    perror("Cannot chdir to traces directory");
    abort();
  }

  traces_dir = opendir("./");
  if (traces_dir == NULL) {
    perror("Cannot open to traces directory");
    abort();
  }

  while ((directory_entry = readdir(traces_dir))) {
    stat(directory_entry->d_name, &file_stats);

    if (! S_ISREG(file_stats.st_mode))
      continue;

    // Extract key, message and ciphertext from the filename
    key = strstr(directory_entry->d_name, "k=");
    message = strstr(directory_entry->d_name, "m=");
    ciphertext = strstr(directory_entry->d_name, "c=");

    if ((key == NULL) || (message == NULL) || (ciphertext == NULL))
      continue;

    trace = (trace_s *) malloc(sizeof(trace_s));

    sscanf(key + 2,
	   "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
	   &trace->key[0], &trace->key[1], &trace->key[2], &trace->key[3],
	   &trace->key[4], &trace->key[5], &trace->key[6], &trace->key[7]);

    sscanf(message + 2,
	   "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
	   &trace->message[0], &trace->message[1], &trace->message[2],
	   &trace->message[3], &trace->message[4], &trace->message[5],
	   &trace->message[6], &trace->message[7]);

    sscanf(ciphertext + 2,
	   "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
	   &trace->ciphertext[0], &trace->ciphertext[1],
	   &trace->ciphertext[2], &trace->ciphertext[3],
	   &trace->ciphertext[4], &trace->ciphertext[5],
	   &trace->ciphertext[6], &trace->ciphertext[7]);

    read_trace(directory_entry->d_name, trace);
  verify_des(trace);
    if (trace->num_samples > 0)
      add_trace_to_list(trace);
    else
      free(trace);
  }
}
