#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *byte_to_binary(char x);
unsigned char reverse(unsigned char b);
int copy_file(char *inpath, char *outpath);
const char *get_filename_ext(const char *filename);

#endif