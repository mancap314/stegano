 #include "stegano.h"
#include "utils.h"
#include "converters.h"
#include "lsb.h"

size_t get_max_size(char *fp, bool tobmp)
{
    const char *ext = get_filename_ext(fp);
    size_t flength = 0;
    bool converted = false;
    char bmp_fp[strlen(fp) + 6];

    if ((!strncmp(ext, "jpg", 3) || !strncmp(ext, "jpeg", 4) || !strncmp(ext, "png", 3)) && tobmp)
    {
        converted = true;
        snprintf(bmp_fp, sizeof(bmp_fp), "%s%s", fp, ".bmp");
        int ret = (!strncmp(ext, "png", 3)) ? png2bmp(fp, bmp_fp) : jpg2bmp(fp, bmp_fp);
        if (ret == EXIT_FAILURE)
        {
            fprintf(stderr, "[ERROR] Could not convert %s to bmp. Exit.\n", fp);
            exit(EXIT_FAILURE);
        }
    }

    FILE *input_file = converted ? fopen(bmp_fp, "rb") : fopen(fp, "rb");

    fseek(input_file, 0, SEEK_END);
    flength = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    if (flength == -1)
    {
        fprintf(stderr, "[ERROR] get_max_size(): could not determine size of %s.Exit.\n", bmp_fp);
        if (converted && remove(bmp_fp) != 0)
            printf("[WARNING] get_max_size(): Could not delete %s.\n", bmp_fp);
        fclose(input_file);
        exit(EXIT_FAILURE);
    }
    else
    {
        fclose(input_file);
        if (converted && remove(bmp_fp) != 0)
            printf("[WARNING] get_max_size(): Could not delete %s.\n", bmp_fp);
    }

    return flength;
}

int emdebed(char *covering_fp, char *tocover_fp, char *output_fp, bool tobmp, bool embed)
{
    const char *ext = get_filename_ext(covering_fp);
    int ret = EXIT_SUCCESS;
    bool converted = false;
    char bmp_fp[strlen(covering_fp) + 6];

    if ((!strncmp(ext, "jpg", 3) || !strncmp(ext, "jpeg", 4) || !strncmp(ext, "png", 3)) && tobmp)
    {
        converted = true;
        snprintf(bmp_fp, sizeof(bmp_fp), "%s%s", covering_fp, ".bmp");
        ret = (!strncmp(ext, "png", 3)) ? png2bmp(covering_fp, bmp_fp) : jpg2bmp(covering_fp, bmp_fp);
        if (ret == EXIT_FAILURE)
        {
            fprintf(stderr, "[ERROR] Could not convert %s to bmp. Exit.\n", covering_fp);
            exit(EXIT_FAILURE);
        }
        printf("[DEBUG] emdebed(): converted %s to %s.\n", covering_fp, bmp_fp);
    }

    if (embed)
    {
        if (converted) {
            char bmp_bmp_fp[strlen(covering_fp) + 10];
            snprintf(bmp_bmp_fp, sizeof(bmp_bmp_fp), "%s%s", bmp_fp, ".bmp");
            // Stegging in first bmp, resulting in second bmp
            lsb_stegging(bmp_fp, tocover_fp, bmp_bmp_fp);
            // Converting second bmp to original format
            ret = (!strncmp(ext, "png", 3)) ? bmp2png(bmp_bmp_fp, output_fp) : bmp2jpg(bmp_bmp_fp, output_fp);
            if (ret == EXIT_FAILURE)
            {
                fprintf(stderr, "[ERROR] Could not convert %s to bmp. Exit.\n", covering_fp);
                exit(EXIT_FAILURE);
            }
            // Remove second bmp
            if (remove(bmp_bmp_fp) != 0)
                printf("[WARNING] emdebed(): Could not delete %s.\n", bmp_bmp_fp);

        } else {
            lsb_stegging(covering_fp, tocover_fp, output_fp);
        }
    }
    else
    {
        ret = converted ? lsb_unstegging(bmp_fp, output_fp) : lsb_unstegging(covering_fp, output_fp);
    }

    if (ret == EXIT_FAILURE)
    {
        fprintf(stderr, "[ERROR] emdebed(): Could not embed %s into %s.\n", tocover_fp, bmp_fp);
        return EXIT_FAILURE;
    }
    else
    {
        if (converted && remove(bmp_fp) != 0)
            printf("[WARNING] emdebed(): Could not delete %s.\n", bmp_fp);
    }

    return EXIT_SUCCESS;
}