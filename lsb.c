/* Implementing LSB (Last Significant Bit) steganography */

#include "lsb.h"

const char *byte_to_binary(char x) {
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1) strncat(b, ((x & z) == z) ? "1" : "0", 1);

    return b;
}


int lsb_stegging(char* covering_fp, char* tocover_fp, char* output_fp) {
    size_t ncprinted = 0;
    // COVERING FILE
    FILE* covering_file = fopen(covering_fp, "rb");
    if (covering_file == NULL) {
        fprintf(stderr, "[ERROR] lsb stegging: failed to open %s.\n", covering_fp);
        return EXIT_FAILURE;
    }
    // Find size of file
    fseek(covering_file, 0, SEEK_END);
    size_t covering_flength = ftell(covering_file);
    fseek(covering_file, 0, SEEK_SET);
    if (covering_flength == -1) {
        fprintf(stderr, "[ERROR] lsb stegging: could not determine size of %s.\n", covering_fp);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    // TOCOVER FILE
    FILE* tocover_file = fopen(tocover_fp, "rb");
    if (tocover_file == NULL) {
        fprintf(stderr, "[ERROR] lsb stegging: failed to open %s.\n", tocover_fp);
        return EXIT_FAILURE;
    }
    // Find size of file
    fseek(tocover_file, 0, SEEK_END);
    size_t tocover_flength = ftell(tocover_file);
    fseek(tocover_file, 0, SEEK_SET);
    if (tocover_flength == -1) {
        fprintf(stderr, "[ERROR] lsb stegging: could not determine size of %s.\n", tocover_fp);
        fclose(tocover_file);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    if ((covering_flength - 1) < (N_BITS_IN_BYTE * tocover_flength)) {
        fprintf(stderr, "[ERROR] lsb stegging: %s has size %llu; but max. %llu can be lsb-stegg'ed in %s of size %llu.\n", tocover_fp, tocover_flength, (size_t)((covering_flength - 1) / N_BITS_IN_BYTE), covering_fp, covering_flength);
        fclose(tocover_file);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    printf("[INFO] lsb stegging: size of %s (covering): %llu ; size of %s (to cover): %llu\n", covering_fp, covering_flength, tocover_fp, tocover_flength);

    // OUTPUT FILE
    FILE* output_file = fopen(output_fp, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "[ERROR] lsb stegging: failed to open %s.\n", output_fp);
        fclose(tocover_file);
        fclose(covering_file);
        return EXIT_FAILURE;
    }

    size_t n_bytes_to_copy = covering_flength - N_BITS_IN_BYTE * tocover_flength;
    unsigned char c_covering;
    while(((c_covering = fgetc(covering_file)) != EOF) && !feof(covering_file) && (ncprinted < n_bytes_to_copy)) {
        fputc(c_covering, output_file); 
        ncprinted++;
    };

    // Start stegging
    unsigned char c_tocover;
    unsigned char c_covering_stagged;
    unsigned char new_value;
    while(((c_tocover = fgetc(tocover_file)) != EOF) && !feof(tocover_file) && (ncprinted <= covering_flength)) {
        // printf("[DEBUG] lsb_stegging(): c_tocover=%s\n", byte_to_binary(c_tocover));
        for (uint8_t i = 0; i < N_BITS_IN_BYTE; i++) {
            new_value = (c_tocover & 1);
            c_covering = fgetc(covering_file);
            printf("[DEBUG] (directly after reading) c_covering=%c, to_binary(c_covering)=%s, hexa=%#02x\n", c_covering, byte_to_binary(c_covering), c_covering);
            c_covering_stagged = (c_covering & ~1) | (new_value & 1);
            printf("[DEBUG] lsb_stegging(): new_value=%s, c_covering=%s, c_covering_stagged=%s\n", byte_to_binary(new_value), byte_to_binary(c_covering), byte_to_binary(c_covering_stagged));
            printf("[DEBUG] lsb_stagging(): c_covering_stagged=%#02x\n", c_covering_stagged);
            fputc(c_covering_stagged, output_file); 
            ncprinted++;
            c_tocover >>=  1;    
        }
    }

    printf("[DEBUG] n printed characters: %llu\n", ncprinted);
    printf("[INFO]: %s lsb-stegg'ed in %s, output in %s\n", tocover_fp, covering_fp, output_fp);

    fclose(covering_file);
    fclose(tocover_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}


int copy_file(char* inpath, char* outpath) {
    FILE *sourceFile, *destFile;
    unsigned char ch;

    // Open the source file in read mode
    sourceFile = fopen(inpath, "rb");
    if (sourceFile == NULL) {
        printf("Unable to open the source file.\n");
        return EXIT_FAILURE;
    }

    // Open the destination file in write mode
    destFile = fopen(outpath, "wb");
    if (destFile == NULL) {
        printf("Unable to create the destination file.\n");
        return EXIT_FAILURE;
    }

    // Copy the contents byte by byte
    size_t n_bytes_copied = 0;
    while (((ch = fgetc(sourceFile)) != EOF) && !feof(sourceFile)) {
        fputc(ch, destFile);
        n_bytes_copied++;
        if (n_bytes_copied % 1000 == 0) printf("%llu kb copied\n", n_bytes_copied / 1000);
    }

    printf("File copied successfully.\n");

    // Close the files
    fclose(sourceFile);
    fclose(destFile);

    return EXIT_SUCCESS;
}