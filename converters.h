#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <stdio.h>
#include <stdlib.h>
// #include <jpeglib.h>
#include <FreeImage.h>

int jpg2bmp(char *input_fp, char *output_fp);
int bmp2jpg(char *input_fp, char *output_fp);
int png2bmp(char *input_fp, char *output_fp);
int bmp2png(char *input_fp, char *output_fp);

#endif