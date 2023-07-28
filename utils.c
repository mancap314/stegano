#include "utils.h"

const char *byte_to_binary(char x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
        strncat(b, ((x & z) == z) ? "1" : "0", 1);

    return b;
}

// Reverse byte
unsigned char reverse(unsigned char b)
{
    // from: https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
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
            printf("%llu kb copied\n", (long long unsigned int)n_bytes_copied / 1000);
    }

    printf("File copied successfully.\n");

    // Close the files
    fclose(sourceFile);
    fclose(destFile);

    return EXIT_SUCCESS;
}

const char *get_filename_ext(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}