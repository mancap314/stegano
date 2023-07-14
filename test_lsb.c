#include "lsb.h"

int test_lsb_stegging()
{
    char *covering_fp = "data/moi-hoodie.bmp";
    char *tocover_fp = "data/message.txt";
    char *output_fp = "data/moi-hoodie-steg.bmp";

    lsb_stegging(covering_fp, tocover_fp, output_fp);
    // copy_file(covering_fp, output_fp);

    return EXIT_SUCCESS;
}

int test_lsb_unstegging()
{
    char *covered_fp = "data/moi-hoodie-steg.bmp";
    char *message_fp = "data/message_read.txt";

    lsb_unstegging(covered_fp, message_fp);

    return EXIT_SUCCESS;
}

int main()
{
    test_lsb_stegging();
    test_lsb_unstegging();

    return EXIT_SUCCESS;
}
