#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int generateBin = 0;

    if (argc < 4)
    {
        printf("Usage: %s <input> -o <output> [-bin]\n", argv[0]);
        return 1;
    }

    if (argc == 5 && strcmp(argv[4], "-bin") == 0)
    {
        generateBin = 1;
    }

    FILE *input = fopen(argv[1], "r");
    char tempFileName[100];
    strcpy(tempFileName, argv[3]);

    if (generateBin)
    {
        strcat(tempFileName, ".asm");
    }

    FILE *output = fopen(tempFileName, "w");

    if (!input || !output)
    {
        printf("Error opening files.\n");
        return 1;
    }

    char tempFilesToDelete[10][100];
    int tempFileIndex = 0;
    char line[256];

    while (fgets(line, sizeof(line), input))
    {
        if (strstr(line, "ADRESS ") == line)
        {
            char name[100], value[100];
            sscanf(line, "ADRESS %s = %s", name, value);
            fprintf(output, "%s equ %s\n", name, value);
        }
        else if (strstr(line, "DEFINE ") == line)
        {
            char name[100], value[100];
            sscanf(line, "DEFINE %s = \"%[^\"]", name, value);
            fprintf(output, "%s db '%s', 0\n", name, value);
        }
        else if (strstr(line, "FUNC ") == line)
        {
            char name[100];
            sscanf(line, "FUNC %[^{]", name);
            fprintf(output, "%s:\n", name);
        }
        else if (strcmp(line, "BITS 32\n") == 0)
        {
            fprintf(output, "[bits 32]\n");
        }
        else if (strcmp(line, "BITS 16\n") == 0)
        {
            fprintf(output, "[bits 16]\n");
        }
        else if (strcmp(line, "BITS 64\n") == 0)
        {
            fprintf(output, "[bits 64]\n");
        }
        else if (strcmp(line, "CODE\n") == 0)
        {
            fprintf(output, "section .text\n");
        }
        else if (strcmp(line, "DATA\n") == 0)
        {
            fprintf(output, "section .data\n");
        }
        else if (strcmp(line, "BSS\n") == 0)
        {
            fprintf(output, "section .bss\n");
        }
        else if (strstr(line, "PUBLIC ") == line)
        {
            char name[100];
            sscanf(line, "PUBLIC %s", name);
            fprintf(output, "global %s\n", name);
        }
        else if (strstr(line, "INCLUDE ") == line)
        {
            char includedFileName[100];
            sscanf(line, "INCLUDE \"%[^\"]\"", includedFileName);

            char tempIncludedFile[120];
            sprintf(tempIncludedFile, "%s_tmp.asm", includedFileName);
            char command[200];
            sprintf(command, "%s %s -o %s", argv[0], includedFileName, tempIncludedFile);
            system(command);

            fprintf(output, "%%include \"%s\"\n", tempIncludedFile);

            strcpy(tempFilesToDelete[tempFileIndex], tempIncludedFile);
            tempFileIndex++;
        }
        else if (strstr(line, "CALL ") == line)
        {
            char functionName[100];
            sscanf(line, "CALL %s", functionName);
            fprintf(output, "call %s\n", functionName);
        }
        else if (strcmp(line, "RETURN\n") == 0)
        {
            fprintf(output, "ret\n");
        }
        else
        {
            char address[100], offset[10], value[10];
            if (sscanf(line, " %[^()](%[^)]) = '%[^']'", address, offset, value) == 3)
            {
                if (strcmp(offset, "0") == 0)
                {
                    fprintf(output, "mov byte [%s], '%s'\n", address, value);
                }
                else
                {
                    int offset_val = atoi(offset);
                    fprintf(output, "mov byte [%s + %d], '%s'\n", address, offset_val * 2, value);
                }
            }
        }

        if (strstr(line, "HALT\n") != NULL)
        {
            fprintf(output, "cli\nhang:\nhlt\njmp hang\n");
        }
    }

    fclose(input);
    fclose(output);

    if (generateBin)
    {
        char command[200];
        sprintf(command, "nasm -f bin %s -o %s", tempFileName, argv[3]);
        system(command);
        remove(tempFileName);
    }

    for (int i = 0; i < tempFileIndex; i++)
    {
        remove(tempFilesToDelete[i]);
    }

    return 0;
}
