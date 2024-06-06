#include "set.h"

int main(int argc, char *argv[]) {
    FILE *inputFile = NULL;
    int stopCommandGiven = 0;
    int running = 1;
    char input[256];
    Set sets[6]; /* Declare an array to hold the six sets */
    Command commands[] = { /* Define the commands array */
        {"print_set", cmd_print_set, 1},
        {"union_set", cmd_union_set, 3},
        {"intersect_set", cmd_intersect_set, 3},
        {"sub_set", cmd_sub_set, 3},
        {"symdiff_set", cmd_symdiff_set, 3},
        {"read_set", help_read_set, -1},  /* -1 indicates variable number of arguments */
        {"stop", (CommandFunc)stop, 0},
        {NULL, NULL, 0}  /* End of commands marker */
    };

    /* Initialize sets to 0 */
    memset(sets, 0, sizeof(sets));

    if (argc > 1) {
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            printf("Could not open input file %s\n", argv[1]);
            return 1;
        }
    }

    /* Read commands from the user or file in a loop */
    while (running) {
        if (inputFile != NULL) {
            if (fgets(input, sizeof(input), inputFile) == NULL) {
                break;
            }
            /* Remove the newline character at the end if present */
            input[strcspn(input, "\n")] = '\0';
        } else {
            printf("\nEnter a command > ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            /* Remove the newline character at the end if present */
            input[strcspn(input, "\n")] = '\0';
        }
    
        parseAndExecuteCommand(input, sets, &running, commands);
        
        if (strcmp(input, "stop") == 0) {
            stopCommandGiven = 1;
        }
    }

    if (inputFile != NULL) {
        fclose(inputFile);
    }

    if (!stopCommandGiven) {
        printf("No stop command was given, program will terminate automatically\n");
        return 1;
    }
    return 0;
}
