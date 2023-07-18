#include "converters.h"

int test_jpg2bmp()
{
    char *input_fp = "data/fractal_2.jpg";
    char *output_fp = "data/fractal_2.bmp";
    int ret = jpg2bmp(input_fp, output_fp);
    return ret;
}

int test_bmp2jpg()
{
    char *input_fp = "data/fractal_2.bmp";
    char *output_fp = "data/fractal_2_back.jpg";
    int ret = bmp2jpg(input_fp, output_fp);
    return ret;
}

int test_png2bmp()
{
    char *input_fp = "data/fractal.png";
    char *output_fp = "data/fractal.bmp";
    int ret = png2bmp(input_fp, output_fp);
    return ret;
}

int test_bmp2png()
{
    char *input_fp = "data/fractal.bmp";
    char *output_fp = "data/fractal.png";
    int ret = bmp2png(input_fp, output_fp);
    return ret;
}

int main()
{
    int ret;

    ret = test_jpg2bmp();
    printf("[INFO] test_jpg2bmp() %s\n", (ret == EXIT_SUCCESS) ? "succeeded" : "failed");

    ret = test_bmp2jpg();
    printf("[INFO] test_jpg2bmp() %s\n", (ret == EXIT_SUCCESS) ? "succeeded" : "failed");

    ret = test_png2bmp();
    printf("[INFO] test_png2bmp() %s\n", (ret == EXIT_SUCCESS) ? "succeeded" : "failed");

    ret = test_bmp2png();
    printf("[INFO] test_bmp2png() %s\n", (ret == EXIT_SUCCESS) ? "succeeded" : "failed");
}