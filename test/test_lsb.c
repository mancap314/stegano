#include "lsb.h"

int test_lsb_stegging()
{
    printf("[INFO] test_lsb_stegging()\n");
    char *covering_fp = "data/fractal.bmp";
    char *tocover_fp = "data/message.txt";
    char *output_fp = "data/fractal.bmp";
    int ret;

    ret = lsb_stegging(covering_fp, tocover_fp, output_fp);
    printf(
        "[INFO] Covering %s in %s: result stored in %s. %s\n",
        tocover_fp,
        covering_fp,
        output_fp,
        (ret == EXIT_SUCCESS) ? "OK" : "FAILED");
    // copy_file(covering_fp, output_fp);

    return EXIT_SUCCESS;
}

int test_lsb_unstegging()
{
    printf("[INFO] test_lsb_unstegging()\n");
    char *covered_fp = "data/fractal.bmp";
    char *message_fp = "data/message_read";
    int ret;

    ret = lsb_unstegging(covered_fp, message_fp);
    printf(
        "[INFO] Uncovering %s: result stored in %s.txt %s\n",
        covered_fp,
        message_fp,
        (ret == EXIT_SUCCESS) ? "OK" : "FAILED");

    return EXIT_SUCCESS;
}

int main()
{
    test_lsb_stegging();
    test_lsb_unstegging();

    return EXIT_SUCCESS;
}
