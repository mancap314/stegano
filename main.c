#include "converters.h"
#include "lsb.h"
#include "stegano.h"
#include "utils.h"

int main(int argc, char *argv[argc + 1])
{
    char *usage = "Stegano\nTool for LSB steganography.\nParameters:\n"
                  "\t-c --covering: path to the file that will contain (insertion) / already containing (extraction) file to cover. Mandatory.\n"
                  "\t-t --tocover: path to the file to cover. Mandatory for insertion, ignored for extraction\n"
                  "\t-o --output: path of the file that will contain the inserted file (insertion) / to the file containing the extracted file (extraction). Mandatory.\n"
                  "\t-a --action: either \"insertion\" or \"extraction\". Default: \"extraction\"\n"
                  "\t-b --bmp: flag (no value). Converts input file to bmp format. In case of insertion, the input must be in jpg/jpeg or png format, the output will be in png. In case of extraction, the input must be in png format. Default: no bmp conversion.\n"
                  "\t-h --help: print this message.\n";

    uint8_t covering_ind = 0, tocover_ind = 0, output_ind = 0;
    bool insertion = false, tobmp = false;

    for (uint8_t i = 1; i < argc; i++)
    {
        if (!strncmp(argv[i], "-h", 3) || !strncmp(argv[i], "--help", 7))
        {
            printf("%s", usage);
            return EXIT_SUCCESS;
        }
        if (!strncmp(argv[i], "-c", 3) || !strncmp(argv[i], "--covering", 11))
        {
            i++;
            covering_ind = i;
        }
        if (!strncmp(argv[i], "-t", 3) || !strncmp(argv[i], "--tocover", 10))
        {
            i++;
            tocover_ind = i;
        }
        if (!strncmp(argv[i], "-o", 3) || !strncmp(argv[i], "--output", 9))
        {
            i++;
            output_ind = i;
        }
        if (!strncmp(argv[i], "-a", 3) || !strncmp(argv[i], "--action", 9))
        {
            i++;
            if (!strncmp(argv[i], "insertion", 10))
            {
                insertion = true;
            }
            else if (!strncmp(argv[i], "extraction", 11))
            {
                insertion = false;
            }
            else
            {
                fprintf(stderr, "[ERROR] The value for the argument %s must be \"insertion\" or \"extraction\".\n", argv[i - 1]);
                exit(EXIT_FAILURE);
            }
        }
        if (!strncmp(argv[i], "-b", 3) || !strncmp(argv[i], "--bmp", 6))
        {
            tobmp = true;
        }
    }

    if (covering_ind == 0)
    {
        perror("[ERROR] The argument -c / --covering is mandatory.");
        printf("%s", usage);
        exit(EXIT_FAILURE);
    }
    if (output_ind == 0)
    {
        perror("[ERROR] The argument -o / --output is mandatory.");
        printf("%s", usage);
        exit(EXIT_FAILURE);
    }
    if (insertion && tocover_ind == 0)
    {
        perror("[ERROR] The argument -t / --tocover is mandatory when inserting.");
        printf("%s", usage);
        exit(EXIT_FAILURE);
    }
    if (!insertion && tocover_ind > 0)
    {
        printf("[WARNING] argument -t / --tocover ignored when extracting.\n");
    }

    // Print info message tellig what will be done
    if (insertion)
    {
        printf("[INFO] Covering %s into %s with%s bmp conversion, result in %s\n", argv[tocover_ind], argv[covering_ind], tobmp ? "" : " no", argv[output_ind]);
    }
    else
        printf("[INFO] Extracting from %s with%s bmp conversion, result in %s\n", argv[covering_ind], tobmp ? "" : " no", argv[output_ind]);

    int ret = emdebed(argv[covering_ind], argv[tocover_ind], argv[output_ind], tobmp, insertion);

    char message[300];
    if (insertion)
    {
        snprintf(message, 300, "[%s] File %s covered into file %s. Result in %s. %s.\n",
                 (ret == EXIT_SUCCESS) ? "INFO" : "ERROR", argv[tocover_ind], argv[covering_ind], argv[output_ind], (ret == EXIT_SUCCESS) ? "OK" : "FAILED");
        (ret == EXIT_SUCCESS) ? printf("%s", message) : fprintf(stderr, "%s", message);
    }
    else
    {
        snprintf(message, 300, "[%s] Extraction of %s from %s %s.\n",
                 (ret == EXIT_SUCCESS) ? "INFO" : "ERROR", argv[output_ind], argv[covering_ind], (ret == EXIT_SUCCESS) ? "OK" : "FAILED");
        (ret == EXIT_SUCCESS) ? printf("%s", message) : fprintf(stderr, "%s", message);
    }

    return ret;
}