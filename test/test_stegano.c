#include "stegano.h"

int test_get_max_size()
{
    printf("[INFO] test_get_max_size()\n");
    char *png_fp = "data/fractal.png";
    bool tobmp = true;
    size_t max_size = get_max_size(png_fp, tobmp);
    printf("[INFO]: %s with tobmp=%s, max_size=%lu\n", png_fp, tobmp ? "true" : "false", max_size);
    return EXIT_SUCCESS;
}

int test_emdebed()
{
    printf("[INFO] test_embeded()\n");
    char *covering_fp = "data/fractal_2.jpg";
    char *tocover_fp = "data/message.txt";
    char *output_fp = "data/fractal_2_with_message.jpg";
    bool tobmp = true;
    bool embed = true;
    int ret;

    ret = emdebed(covering_fp, tocover_fp, output_fp, tobmp, embed);
    printf(
        "[INFO] %s embedded in %s with tobmp=%s, result in %s: %s.\n",
        tocover_fp,
        covering_fp,
        tobmp ? "true" : "false",
        output_fp,
        (ret == EXIT_SUCCESS) ? "OK" : "FAILED");

    char *output_debed_fp = "data/message_extracted";
    ret = emdebed(output_fp, "", output_debed_fp, tobmp, embed);
    printf(
        "[INFO] %s debedded with tobmp=%s, result in %s: %s.\n",
        tocover_fp,
        tobmp ? "true" : "false",
        output_fp,
        (ret == EXIT_SUCCESS) ? "OK" : "FAILED");

    return EXIT_SUCCESS;
}

int main()
{
    test_get_max_size();
    test_emdebed();

    return EXIT_SUCCESS;
}