#include "converters.h"

int main() {
    char* input_jpg_fp = "data\\moi_costume.jpeg";
    char* output_bmp_fp = "data\\moi_costume.bmp";
    char* output_jpg_fp = "data\\moi_costume_back.jpeg";

    jpg2bmp(input_jpg_fp, output_bmp_fp);
    jpg2bmp(output_bmp_fp, output_jpg_fp);

    return EXIT_SUCCESS;
}