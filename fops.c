#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 1024

int main(int argc, char *argv[]) {
    // Check for the correct number of arguments
    if (argc != 4) {
        printf("How to Use: %s <command> <source> <destination>\n", argv[0]);
        return 1;
    }
    char *command = argv[1];
    char *source = argv[2];
    char *dest = argv[3];

    // Copy
    if (strcmp(command, "copy") == 0) {
        FILE *input = fopen(source, "r");
        if (input == NULL) {
            printf("Unable to open %s for reading\n", source);
            return 1;
        }

        //Output File
        FILE *output = fopen(dest, "w");
        if (output == NULL) {
            printf("Unable to open %s for writing\n", dest);
            return 1;
        }

        // Copy
        char c;
        while ((c = fgetc(input)) != EOF) {
            fputc(c, output);
        }
        fclose(input);
        fclose(output);

    //Rename
    } else if (strcmp(command, "rename") == 0) {
        if (rename(source, dest) != 0) {
            printf("Unable to rename %s to %s\n", source, dest);
            return 1;
        }
    //Delete
    } else if (strcmp(command, "delete") == 0) {
        if (remove(source) != 0) {
            printf("Unable to delete %s\n", source);
            return 1;
        }
    //Convert csv-> json or json->csv
    } else if (strcmp(command, "convert") == 0) {
        char *extension = strrchr(source, '.');

        if (extension == NULL) {
            printf("Unable to determine file extension for %s\n", source);
            return 1;
        }

        //csv->json
        if (strcmp(extension, ".csv") == 0) {
            FILE *input = fopen(source, "r");
            if (input == NULL) {
                printf("Unable to open %s for reading\n", source);
                return 1;
            }

            FILE *output = fopen(dest, "w");
            if (output == NULL) {
                printf("Unable to open %s for writing\n", dest);
                return 1;
            }
            char buffer[BUFFER];

            while (fgets(buffer, BUFFER, input) != NULL) {
                // Tokenize by comma
                char *token = strtok(buffer, ",");
                int i = 0;
                char *values[10];
                while (token!= NULL) {
                    values[i++] = token;
                    token = strtok(NULL, ",");
                }

                // Print as a JSON object
                fprintf(output, "{");
                for (int j = 0; j < i; j++) {
                    fprintf(output, "\"field%d\": \"%s\"", j + 1, values[j]);
                    if (j < i - 1) {
                        fprintf(output, ",");
                    }
                }
                fprintf(output, "}\n");
            }
            fclose(input);
            fclose(output);

        //json -> csv
        } else if (strcmp(extension, ".json") == 0) {
            FILE *input = fopen(source, "r");
                        if (input == NULL) {
                printf("Unable to open %s for reading\n", source);
                return 1;
            }
            FILE *output = fopen(dest, "w");
            if (output == NULL) {
                printf("Unable to open %s for writing\n", dest);
                return 1;
            }

            char buffer[BUFFER];
            while (fgets(buffer, BUFFER, input) != NULL) {
                //Parse through the json object
                int i = 0;
                char *keys[10];
                char *vals[10];
                char *token = strtok(buffer, "\":,");
                while (token != NULL) {
                    if (i % 2 == 0) {
                        // Token is a key
                        keys[i / 2] = token;
                    } else {
                        // Token is a value
                        vals[i / 2] = token;
                    }
                    i++;
                    token = strtok(NULL, "\":,");
                }

                // Print as a CSV
                for (int j = 0; j < i / 2; j++) {
                    fprintf(output, "\"%s\"", vals[j]);
                    if (j < i / 2 - 1) {
                        fprintf(output, ",");
                    }
                }
                fprintf(output, "\n");
            }
            fclose(input);
            fclose(output);
        } 
        //EDGE
        else {
            printf("Unsupported file extension: %s\n", extension);
            return 1;
        }
    } else {
        printf("Invalid command: %s\n", command);
        return 1;
    }
    return 0;
}
