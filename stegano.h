#ifndef STEGANO_H
#define STEGANO_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

size_t get_max_size(char *fp, bool tobmp);
int emdebed(char *covering_fp, char *tocover_fp, char *output_fp, bool tobmp, bool embed);

#endif