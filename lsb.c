/* Implementing LSB (Last Significant Bit) steganography */

#include "lsb.h"
#include "utils.h"

int lsb_stegging(char *covering_fp, char *tocover_fp, char *output_fp)
{
    printf("[INFO] lsb_stegging(): covering_fp=%s, tocover_fp=%s, output_fp=%s\n", covering_fp, tocover_fp, output_fp);
    size_t ncprinted = 0;
    // COVERING FILE
    FILE *covering_file = fopen(covering_fp, "rb");
    if (covering_file == NULL)
    {
        fprintf(stderr, "[ERROR] lsb stegging: failed to open %s.\n", covering_fp);
        return EXIT_FAILURE;
    }
    // Find size of file
    fseek(covering_file, 0, SEEK_END);
    size_t covering_flength = ftell(covering_file);
    fseek(covering_file, 0, SEEK_SET);
    if (covering_flength == -1)
    {
        fprintf(stderr, "[ERROR] lsb stegging: could not determine size of %s.\n", covering_fp);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    // TOCOVER FILE
    FILE *tocover_file = fopen(tocover_fp, "rb");
    if (tocover_file == NULL)
    {
        fprintf(stderr, "[ERROR] lsb stegging: failed to open %s.\n", tocover_fp);
        return EXIT_FAILURE;
    }
    // Find size of file
    fseek(tocover_file, 0, SEEK_END);
    size_t tocover_flength = ftell(tocover_file);
    fseek(tocover_file, 0, SEEK_SET);
    if (tocover_flength == -1)
    {
        fprintf(stderr, "[ERROR] lsb stegging: could not determine size of %s.\n", tocover_fp);
        fclose(tocover_file);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    if ((covering_flength - 1) < (N_BITS_IN_BYTE * (tocover_flength + CHFORFS + CHFOREXT)))
    {
        fprintf(stderr, "[ERROR] lsb stegging(): %s has size %llu; but max. %llu can be lsb-stegg'ed in %s of size %llu.\n",
                tocover_fp,
                (long long unsigned int)tocover_flength,
                (long long unsigned int)((covering_flength - 1) / N_BITS_IN_BYTE - CHFORFS - CHFOREXT),
                covering_fp,
                (long long unsigned int)covering_flength);
        fclose(tocover_file);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    printf("[INFO] lsb stegging(): size of %s (covering): %llu ; size of %s (to cover): %llu\n",
           covering_fp,
           (long long unsigned int)covering_flength,
           tocover_fp,
           (long long unsigned int)tocover_flength);

    // OUTPUT FILE
    FILE *output_file = fopen(output_fp, "wb");
    if (output_file == NULL)
    {
        fprintf(stderr, "[ERROR] lsb stegging(): failed to open %s.\n", output_fp);
        fclose(tocover_file);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    size_t n_bytes_to_copy = covering_flength - (N_BITS_IN_BYTE * (tocover_flength + CHFORFS + CHFOREXT));
    unsigned char c_covering;
    while (((c_covering = fgetc(covering_file)) != EOF) && !feof(covering_file) && (ncprinted < n_bytes_to_copy))
    {
        fputc(c_covering, output_file);
        ncprinted++;
    };

    // Start stegging
    unsigned char c_tocover;
    unsigned char c_covering_stagged;
    unsigned char new_value;
    while (((c_tocover = fgetc(tocover_file)) != EOF) && !feof(tocover_file) && (ncprinted <= covering_flength - (CHFORFS + CHFOREXT) * N_BITS_IN_BYTE))
    {
        for (uint8_t i = 0; i < N_BITS_IN_BYTE; i++)
        {
            new_value = (c_tocover & 1);
            c_covering = fgetc(covering_file);
            c_covering_stagged = (c_covering & ~1) | (new_value & 1);
            fputc(c_covering_stagged, output_file);
            ncprinted++;
            c_tocover >>= 1;
        }
    }

    // Writing the file extension
    const char *input_ext = get_filename_ext(tocover_fp);
    char extarr[CHFOREXT + 1] = {0};
    snprintf(extarr, CHFOREXT + 1, "%s", input_ext);
    uint8_t extarr_ind = 0;
    while ((extarr_ind < CHFOREXT) && (ncprinted <= covering_flength))
    {
        c_tocover = extarr[extarr_ind];
        for (uint8_t i = 0; i < N_BITS_IN_BYTE; i++)
        {
            new_value = (c_tocover & 1);
            c_covering = fgetc(covering_file);
            c_covering_stagged = (c_covering & ~1) | (new_value & 1);
            fputc(c_covering_stagged, output_file);
            ncprinted++;
            c_tocover >>= 1;
        }
        extarr_ind++;
    }

    // Writing the number of bytes to read
    char fsarr[CHFORFS + 1] = {0}; // containing tocover_flength as a string
    snprintf(fsarr, CHFORFS + 1, "%llu", (long long unsigned int)tocover_flength);
    uint8_t fsarr_ind = 0;
    while ((fsarr_ind < CHFORFS) && (ncprinted <= covering_flength))
    {
        c_tocover = fsarr[fsarr_ind];
        for (uint8_t i = 0; i < N_BITS_IN_BYTE; i++)
        {
            new_value = (c_tocover & 1);
            c_covering = fgetc(covering_file);
            c_covering_stagged = (c_covering & ~1) | (new_value & 1);
            fputc(c_covering_stagged, output_file);
            ncprinted++;
            c_tocover >>= 1;
        }
        fsarr_ind++;
    }

    printf("[INFO] lsb stegging(): %s lsb-stegg'ed in %s, output in %s\n", tocover_fp, covering_fp, output_fp);

    fclose(covering_file);
    fclose(tocover_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}

int lsb_unstegging(char *input_fp, char *output_fp)
{
    printf("[INFO] lsb_unstegging(): input_fp=%s, output_fp=%s\n", input_fp, output_fp);
    FILE *input_file = fopen(input_fp, "rb");
    if (input_file == NULL)
    {
        fprintf(stderr, "[ERROR] lsb_unstegging(): Could not open input file at %s\n", input_fp);
        return EXIT_FAILURE;
    }

    // Find size of file
    fseek(input_file, 0, SEEK_END);
    size_t input_flength = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    if (input_flength == -1)
    {
        fprintf(stderr, "[ERROR] lsb_unstegging(): could not determine size of %s.\n", input_fp);
        fclose(input_file);
        return EXIT_FAILURE;
    }

    // Read message extension and size
    if (input_flength < (CHFORFS + CHFOREXT) * N_BITS_IN_BYTE)
    {
        fprintf(stderr, "[ERROR] lsb_unstegging(): reading message length: size of %s : %llu < %u\n",
                input_fp,
                (long long unsigned int)input_flength,
                CHFORFS * N_BITS_IN_BYTE);
        return EXIT_FAILURE;
    }
    unsigned char ch;
    size_t ncread = 0; // number of read characters
    size_t read_ind = 0;
    char extarr[CHFOREXT + 1] = {0};
    char fsarr[CHFORFS + 1] = {0};
    unsigned char extracted_ch = 0;
    fseek(input_file, (input_flength - 2 - (CHFORFS + CHFOREXT) * N_BITS_IN_BYTE), SEEK_SET);
    while (((ch = fgetc(input_file)) != EOF) && !feof(input_file) && (ncread < CHFORFS * N_BITS_IN_BYTE))
    {
        extracted_ch = extracted_ch | (ch & 1);
        extracted_ch <<= 1;
        if (((ncread % N_BITS_IN_BYTE) == 0) && ncread > 0)
        {
            extracted_ch = reverse(extracted_ch);
            read_ind = ncread / N_BITS_IN_BYTE - 1;
            if (read_ind < CHFOREXT)
            {
                extarr[read_ind] = extracted_ch;
            }
            else
            {
                fsarr[read_ind - CHFOREXT] = extracted_ch;
            }
            if ((extracted_ch == '\0') && (read_ind > CHFOREXT))
                break;
            extracted_ch = 0;
        }
        ncread++;
    }

    size_t message_size = (size_t)atoi(fsarr);
    printf("[INFO] lsb_unstegging(): message_extension=%s, message_size=%llu\n", extarr, (long long unsigned int)message_size);
    if (input_flength < (message_size + CHFORFS) * N_BITS_IN_BYTE)
    {
        fprintf(stderr, "[ERROR] lsb_unstegging(): size of %s: %llu too small for message size: %llu\n",
                input_fp,
                (long long unsigned int)input_flength,
                (long long unsigned int)message_size);
        return EXIT_FAILURE;
    }

    // Read Message and write it in the output
    ncread = 0;
    FILE *output_file;
    char output_fp_ext[strlen(output_fp) + 1 + CHFOREXT + 1];
    if (strlen(extarr) == 0)
    {
        output_file = fopen(output_fp, "wb");
    }
    else // file extension is appended to `output_fp`, that (therefore) doesn't need to have the file extension
    {
        snprintf(output_fp_ext, sizeof(output_fp_ext), "%s.%s", output_fp, extarr);
        output_file = fopen(output_fp_ext, "wb");
    }
    if (output_file == NULL)
    {
        fprintf(stderr, "[ERROR] lsb_unstegging(): Could not open output file at %s\n", (strlen(extarr) == 0) ? output_fp : output_fp_ext);
        return EXIT_FAILURE;
    }
    fseek(input_file, (input_flength - 2 - (message_size + CHFORFS + CHFOREXT) * N_BITS_IN_BYTE), SEEK_SET);
    while (((ch = fgetc(input_file)) != EOF) && !feof(input_file) && (ncread < message_size * N_BITS_IN_BYTE))
    {
        extracted_ch = extracted_ch | (ch & 1);
        extracted_ch <<= 1;
        if (((ncread % N_BITS_IN_BYTE) == 0) && ncread > 0)
        {
            extracted_ch = reverse(extracted_ch);
            read_ind = ncread / N_BITS_IN_BYTE - 1;
            fputc(extracted_ch, output_file);
            extracted_ch = 0;
        }
        ncread++;
    }

    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}