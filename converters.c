#include "converters.h"

int jpg2bmp(char *input_fp, char *output_fp)
{

    // Load the image using FreeImage
    FIBITMAP *image = FreeImage_Load(FIF_JPEG, input_fp, JPEG_DEFAULT);
    if (!image)
    {
        printf("Failed to load image: %s\n", input_fp);
        return 1;
    }

    // Convert the image to 24-bit bitmap format
    FIBITMAP *bitmap = FreeImage_ConvertTo24Bits(image);
    FreeImage_Unload(image);

    // Save the bitmap to a file
    if (!FreeImage_Save(FIF_BMP, bitmap, output_fp, BMP_DEFAULT))
    {
        printf("Failed to save image: %s\n", output_fp);
        FreeImage_Unload(bitmap);
        return EXIT_FAILURE;
    }

    FreeImage_Unload(bitmap);

    printf("Image converted successfully: from %s to %s\n", input_fp, output_fp);

    return EXIT_SUCCESS;
}

int bmp2jpg(char *input_fp, char *output_fp)
{

    // Load the image using FreeImage
    FIBITMAP *image = FreeImage_Load(FIF_BMP, input_fp, BMP_DEFAULT);
    if (!image)
    {
        printf("Failed to load image: %s\n", input_fp);
        return EXIT_FAILURE;
    }

    // Convert the image to 24-bit bitmap format
    FIBITMAP *bitmap = FreeImage_ConvertTo24Bits(image);
    FreeImage_Unload(image);

    // Save the bitmap to a JPG file
    if (!FreeImage_Save(FIF_JPEG, bitmap, output_fp, JPEG_QUALITYGOOD))
    {
        printf("Failed to save image: %s\n", output_fp);
        FreeImage_Unload(bitmap);
        return EXIT_FAILURE;
    }

    FreeImage_Unload(bitmap);

    printf("Image converted successfully: from %s to %s\n", input_fp, output_fp);

    return EXIT_FAILURE;
}

// Install libjpeg and verify
/*
int jpg2bmp(char* input_fp, char* output_fp) {
    FILE *inputFile, *outputFile;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPARRAY buffer;
    int rowStride;

    // Open the input JPEG file
    inputFile = fopen(input_fp, "rb");
    if (inputFile == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        return 1;
    }

    // Initialize the JPEG decompression object
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // Set the source file
    jpeg_stdio_src(&cinfo, inputFile);

    // Read the JPEG header
    jpeg_read_header(&cinfo, TRUE);

    // Start the decompression process
    jpeg_start_decompress(&cinfo);

    // Prepare the output BMP file
    outputFile = fopen(output_fp, "wb");
    if (outputFile == NULL) {
        fprintf(stderr, "Error creating output file.\n");
        return 1;
    }

    // Calculate the row stride (bytes per scanline)
    rowStride = cinfo.output_width * cinfo.output_components;

    // Allocate a buffer for storing scanlines
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, rowStride, 1);

    // Write the BMP file header
    unsigned char bmpHeader[54] = {
        0x42, 0x4D, 0x36, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
        0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0xC8, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x30, 0x00, 0x00, 0x0C, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    fwrite(bmpHeader, sizeof(unsigned char), 54, outputFile);

    // Read scanlines and write to the BMP file
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        fwrite(buffer[0], sizeof(unsigned char), rowStride, outputFile);
    }

    // Finish decompression and release resources
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(inputFile);
    fclose(outputFile);

    printf("JPEG to BMP conversion completed.\n");

    return EXIT_SUCCESS;
}
*/
