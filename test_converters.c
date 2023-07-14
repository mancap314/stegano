#include "converters.h"

int main() {
    char* input_jpg_fp = "data/moi-hoodie.jpg";
    char* output_bmp_fp = "data/moi-hoodie.bmp";
    char* output_jpg_fp = "data/moi-hoodie-back.jpg";

    jpg2bmp(input_jpg_fp, output_bmp_fp);
    bmp2jpg(output_bmp_fp, output_jpg_fp);

    return EXIT_SUCCESS;
}
