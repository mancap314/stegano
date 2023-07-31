#ifndef LSB_H
#define LSB_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define N_BITS_IN_BYTE 8
#define BUFFER_SIZE 1000
// Number of characters for file size: CHFORFS
#define CHFORFS 10
// Number of characters for file extension: CHFOREXT
#define CHFOREXT 4

int lsb_stegging(char *covering_fp, char *tocover_fp, char *output_fp);
int lsb_unstegging(char *input_fp, char *output_fp);

#endif