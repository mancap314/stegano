#include "converters.h"
#include "lsb.h"
#include "stegano.h"
#include "utils.h"

int main(int argc, char *argv[argc + 1])
{
    char *usage = "Stegano\nTool for LSB steganography.\nParameters:\n"
                  "\t-c --covering: path to the file that will contain (insertion) / already containing (extraction) file to cover.\n"
                  "\t-t --tocover: path to the file to cover (ignored for extraction).\n"
                  "\t-o --output: path of the file that will contain the inserted file (insertion) / to the file containing the extracted file (extraction).\n"
                  "\t-a --action: either \"insertion\" or \"extraction\".\n"
                  "\t-b --bmp: flag (no value). In case of a jpg/jpeg or png file, will insert/extract to/from the covering file converted to bmp format.\n"
                  "\t-h --help: print this message.\n";

    uint8_t covering_ind = 0, tocover_ind = 0, output_ind = 0;
    bool insertion = false, tobmp = false;

    for (uint8_t i = 1; i < argc + 1; i++)
    {
        if (!strncmp(argv[i], "-h", 2) || !strncmp(argv[i], "--help", 6))
        {
            printf("%s", usage);
            return EXIT_SUCCESS;
        }
        if (!strncmp(argv[i], "-c", 2) || !strncmp(argv[i], "--covering", 10))
        {
            i++;
            covering_ind = i;
        }
        if (!strncmp(argv[i], "-t", 2) || !strncmp(argv[i], "--tocover", 9))
        {
            i++;
            tocover_ind = i;
        }
        if (!strncmp(argv[i], "-o", 2) || !strncmp(argv[i], "--output", 8))
        {
            i++;
            output_ind = i;
        }
        if (!strncmp(argv[i], "-a", 2) || !strncmp(argv[i], "--action", 8))
        {
            i++;
            if (!strncmp(argv[i], "insertion", 9))
            {
                insertion = true;
            }
            else if (!strncmp(argv[i], "extraction", 10))
            {
                insertion = false;
            }
            else
            {
                fprintf(stderr, "[ERROR] The value for the argument %s must be \"insertion\" or \"extraction\".\n", argv[i - 1]);
                exit(EXIT_FAILURE);
            }
        }
        if (!strncmp(argv[i], "bmp", 3))
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