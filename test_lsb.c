#include "lsb.h"

int main() {
    char* covering_fp = "data\\moi_costume.bmp";
    char* tocover_fp = "data\\message.txt";
    char* output_fp = "data\\moi_costume_steg.bmp";
    
    lsb_stegging(covering_fp, tocover_fp, output_fp);
    // copy_file(covering_fp, output_fp);

    return EXIT_SUCCESS;
}