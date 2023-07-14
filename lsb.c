/* Implementing LSB (Last Significant Bit) steganography */

#include "lsb.h"

const char *byte_to_binary(char x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
        strncat(b, ((x & z) == z) ? "1" : "0", 1);

    return b;
}

unsigned char reverse(unsigned char b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

int lsb_stegging(char *covering_fp, char *tocover_fp, char *output_fp)
{
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

    if ((covering_flength - 1) < (N_BITS_IN_BYTE * (tocover_flength + CHFORFS)))
    {
        fprintf(stderr, "[ERROR] lsb stegging: %s has size %lu; but max. %lu can be lsb-stegg'ed in %s of size %lu.\n", tocover_fp, tocover_flength, (size_t)((covering_flength - 1) / N_BITS_IN_BYTE - CHFORFS), covering_fp, covering_flength);
        fclose(tocover_file);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    printf("[INFO] lsb stegging: size of %s (covering): %lu ; size of %s (to cover): %lu\n", covering_fp, covering_flength, tocover_fp, tocover_flength);

    // OUTPUT FILE
    FILE *output_file = fopen(output_fp, "wb");
    if (output_file == NULL)
    {
        fprintf(stderr, "[ERROR] lsb stegging: failed to open %s.\n", output_fp);
        fclose(tocover_file);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    size_t n_bytes_to_copy = covering_flength - (N_BITS_IN_BYTE * (tocover_flength + CHFORFS));
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
    while (((c_tocover = fgetc(tocover_file)) != EOF) && !feof(tocover_file) && (ncprinted <= covering_flength - CHFORFS * N_BITS_IN_BYTE))
    {
        // printf("[DEBUG] lsb_stegging(): c_tocover=%s\n", byte_to_binary(c_tocover));
        for (uint8_t i = 0; i < N_BITS_IN_BYTE; i++)
        {
            new_value = (c_tocover & 1);
            c_covering = fgetc(covering_file);
            printf("[DEBUG] (directly after reading) c_covering=%c, to_binary(c_covering)=%s, hexa=%#02x\n", c_covering, byte_to_binary(c_covering), c_covering);
            c_covering_stagged = (c_covering & ~1) | (new_value & 1);
            printf("[DEBUG] lsb_stegging(): new_value=%s, c_covering=%s, c_covering_stagged=%s\n", byte_to_binary(new_value), byte_to_binary(c_covering), byte_to_binary(c_covering_stagged));
            printf("[DEBUG] lsb_stagging(): c_covering_stagged=%#02x\n", c_covering_stagged);
            fputc(c_covering_stagged, output_file);
            ncprinted++;
            c_tocover >>= 1;
        }
    }

    // Writing the number of bytes to read
    char fsarr[CHFORFS + 1] = {0}; // containing tocover_flength as a string
    snprintf(fsarr, CHFORFS, "%ld", tocover_flength);
    uint8_t fsarr_ind = 0;
    while ((fsarr_ind < CHFORFS) && (ncprinted <= covering_flength))
    {
        c_tocover = fsarr[fsarr_ind];
        for (uint8_t i = 0; i < N_BITS_IN_BYTE; i++)
        {
            new_value = (c_tocover & 1);
            c_covering = fgetc(covering_file);
            printf("[DEBUG] (writing number of bytes) c_covering=%c, to_binary(c_covering)=%s, hexa=%#02x\n", c_covering, byte_to_binary(c_covering), c_covering);
            c_covering_stagged = (c_covering & ~1) | (new_value & 1);
            printf("[DEBUG] lsb_stegging(): new_value=%s, c_covering=%s, c_covering_stagged=%s\n", byte_to_binary(new_value), byte_to_binary(c_covering), byte_to_binary(c_covering_stagged));
            printf("[DEBUG] lsb_stagging(): c_covering_stagged=%#02x\n", c_covering_stagged);
            fputc(c_covering_stagged, output_file);
            ncprinted++;
            c_tocover >>= 1;
        }
        fsarr_ind++;
    }

    printf("[DEBUG] n printed characters: %lu\n", ncprinted);
    printf("[INFO]: %s lsb-stegg'ed in %s, output in %s\n", tocover_fp, covering_fp, output_fp);

    fclose(covering_file);
    fclose(tocover_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}

int copy_file(char *inpath, char *outpath)
{
    FILE *sourceFile, *destFile;
    unsigned char ch;

    // Open the source file in read mode
    sourceFile = fopen(inpath, "rb");
    if (sourceFile == NULL)
    {
        printf("Unable to open the source file.\n");
        return EXIT_FAILURE;
    }

    // Open the destination file in write mode
    destFile = fopen(outpath, "wb");
    if (destFile == NULL)
    {
        printf("Unable to create the destination file.\n");
        return EXIT_FAILURE;
    }

    // Copy the contents byte by byte
    size_t n_bytes_copied = 0;
    while (((ch = fgetc(sourceFile)) != EOF) && !feof(sourceFile))
    {
        fputc(ch, destFile);
        n_bytes_copied++;
        if (n_bytes_copied % 1000 == 0)
            printf("%lu kb copied\n", n_bytes_copied / 1000);
    }

    printf("File copied successfully.\n");

    // Close the files
    fclose(sourceFile);
    fclose(destFile);

    return EXIT_SUCCESS;
}

int lsb_unstegging(char *input_fp, char *output_fp)
{
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

    // Read message size
    if (input_flength < (CHFORFS * N_BITS_IN_BYTE))
    {
        fprintf(stderr, "[ERROR] lsb_unstegging(): reading message length: size of %s : %lu < %u\n", input_fp, input_flength, CHFORFS * N_BITS_IN_BYTE);
        return EXIT_FAILURE;
    }
    unsigned char ch;
    size_t ncread = 0; // number of read characters
    size_t read_ind = 0;
    char fsarr[CHFORFS + 1] = {0};
    unsigned char extracted_ch = 0;
    fseek(input_file, (input_flength - 2 - CHFORFS * N_BITS_IN_BYTE), SEEK_SET);
    while (((ch = fgetc(input_file)) != EOF) && !feof(input_file) && (ncread < CHFORFS * N_BITS_IN_BYTE))
    {
        extracted_ch = extracted_ch | (ch & 1);
        extracted_ch <<= 1;
        if (((ncread % N_BITS_IN_BYTE) == 0) && ncread > 0)
        {
            extracted_ch = reverse(extracted_ch);
            read_ind = ncread / N_BITS_IN_BYTE - 1;
            fsarr[read_ind] = extracted_ch;
            if (extracted_ch == '\0')
                break;
            printf("[INFO] lsb_unstegging(): reading message size: extracted_ch=%c\n", extracted_ch);
            extracted_ch = 0;
        }
        ncread++;
    }
    fsarr[read_ind + 1] = '\0';

    size_t message_size = (size_t)atoi(fsarr);
    printf("[INFO] lsb_unstegging(): message_size=%lu\n", message_size);
    if (input_flength < (message_size + CHFORFS) * N_BITS_IN_BYTE)
    {
        fprintf(stderr, "[ERROR] lsb_unstegging(): size of %s: %lu too small for message size: %lu\n", input_fp, input_flength, message_size);
        return EXIT_FAILURE;
    }

    // Read Message and write it in the output
    ncread = 0;
    FILE *output_file = fopen(output_fp, "wb");
    if (output_file == NULL)
    {
        fprintf(stderr, "[ERROR] lsb_unstegging(): Could not open output file at %s\n", output_fp);
        return EXIT_FAILURE;
    }
    fseek(input_file, (input_flength - 2 - (message_size + CHFORFS) * N_BITS_IN_BYTE), SEEK_SET);
    while (((ch = fgetc(input_file)) != EOF) && !feof(input_file) && (ncread < message_size * N_BITS_IN_BYTE))
    {
        extracted_ch = extracted_ch | (ch & 1);
        extracted_ch <<= 1;
        if (((ncread % N_BITS_IN_BYTE) == 0) && ncread > 0)
        {
            extracted_ch = reverse(extracted_ch);
            read_ind = ncread / N_BITS_IN_BYTE - 1;
            fputc(extracted_ch, output_file);
            printf("[INFO] lsb_unstegging(): reading message size: extracted_ch=%c\n", extracted_ch);
            extracted_ch = 0;
        }
        ncread++;
    }

    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}