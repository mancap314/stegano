#include "converters.h"

int jpg2bmp(char *input_fp, char *output_fp)
{
    // Load the image using FreeImage
    FIBITMAP *image = FreeImage_Load(FIF_JPEG, input_fp, JPEG_DEFAULT);
    if (!image)
    {
        printf("[ERROR] jpg2bmp(): Failed to load image: %s\n", input_fp);
        return EXIT_FAILURE;
    }

    // Convert the image to 24-bit bitmap format
    FIBITMAP *bitmap = FreeImage_ConvertTo24Bits(image);
    FreeImage_Unload(image);

    // Save the bitmap to a file
    if (!FreeImage_Save(FIF_BMP, bitmap, output_fp, BMP_DEFAULT))
    {
        printf("[ERROR] jpg2bmp(): Failed to save image: %s\n", output_fp);
        FreeImage_Unload(bitmap);
        return EXIT_FAILURE;
    }

    FreeImage_Unload(bitmap);

    printf("[INFO] jpg2bmp(): Image converted successfully: from %s to %s\n", input_fp, output_fp);

    return EXIT_SUCCESS;
}

int bmp2jpg(char *input_fp, char *output_fp)
{
    // Load the image using FreeImage
    FIBITMAP *image = FreeImage_Load(FIF_BMP, input_fp, BMP_DEFAULT);
    if (!image)
    {
        printf("[ERROR] bmp2jpg(): Failed to load image: %s\n", input_fp);
        return EXIT_FAILURE;
    }

    // Convert the image to 24-bit bitmap format
    FIBITMAP *bitmap = FreeImage_ConvertTo24Bits(image);
    FreeImage_Unload(image);

    // Save the bitmap to a JPG file
    if (!FreeImage_Save(FIF_JPEG, bitmap, output_fp, JPEG_QUALITYSUPERB))
    {
        printf("[ERROR] bmp2jpg(): Failed to save image: %s\n", output_fp);
        FreeImage_Unload(bitmap);
        return EXIT_FAILURE;
    }

    FreeImage_Unload(bitmap);

    printf("[INFO] bmp2jpg(): Image converted successfully: from %s to %s\n", input_fp, output_fp);

    return EXIT_SUCCESS;
}

int png2bmp(char *input_fp, char *output_fp)
{
    // Load PNG image using FreeImage
    FIBITMAP *image = FreeImage_Load(FIF_PNG, input_fp, PNG_DEFAULT);
    if (!image)
    {
        fprintf(stderr, "[ERROR] png2bmp(): Could not load image at %s\n", input_fp);
        return EXIT_FAILURE;
    }

    // Convert image to 24-bit BMP
    FIBITMAP *convertedImage = FreeImage_ConvertTo24Bits(image);
    FreeImage_Unload(image);

    // Save BMP image
    if (!FreeImage_Save(FIF_BMP, convertedImage, output_fp, BMP_DEFAULT))
    {
        fprintf(stderr, "[ERROR] png2bmp(): Could not save BMP image at %s\n", output_fp);
        FreeImage_Unload(convertedImage);
        return EXIT_FAILURE;
    }

    FreeImage_Unload(convertedImage);
    printf("[INFO] png2bmp(): Conversion completed successfully from %s to %s\n", input_fp, output_fp);

    return EXIT_SUCCESS;
}

int bmp2png(char *input_fp, char *output_fp)
{
    // Load BMP image using FreeImage
    FIBITMAP *image = FreeImage_Load(FIF_BMP, input_fp, BMP_DEFAULT);
    if (!image)
    {
        fprintf(stderr, "[ERROR] bmp2png(): Failed loading BMP image at %s\n", input_fp);
        return EXIT_FAILURE;
    }

    // Convert image to 24-bit PNG
    FIBITMAP *convertedImage = FreeImage_ConvertTo24Bits(image);
    FreeImage_Unload(image);

    // Save PNG image
    if (!FreeImage_Save(FIF_PNG, convertedImage, output_fp, PNG_DEFAULT))
    {
        fprintf(stderr, "[ERROR] bmp2png(): Failed saving PNG image to %s\n", output_fp);
        FreeImage_Unload(convertedImage);
        return EXIT_FAILURE;
    }

    FreeImage_Unload(convertedImage);
    printf("[INFO] bmp2png(): Conversion completed successfully from %s to %s\n", input_fp, output_fp);

    return EXIT_SUCCESS;
}
