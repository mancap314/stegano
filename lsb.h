#ifndef LSB_H
#define LSB_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define N_BITS_IN_BYTE 8
#define BUFFER_SIZE 1000
// Number of characters for file size: CHFORFS
#define CHFORFS 8

const char *byte_to_binary(char x);
unsigned char reverse(unsigned char b);
int lsb_stegging(char *covering_fp, char *tocover_fp, char *output_fp);
int copy_file(char *inpath, char *outpath);
int lsb_unstegging(char *input_fp, char *output_fp);

#endif