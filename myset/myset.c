#include "set.h"

/* function to check if all set names were inserted correctly */
int getAndValidateSets(char* args[], Set** set1, Set** set2, Set** result, int expectedArgs) {
    char* undefinedSet = NULL;

    /* Validate the first set if expectedArgs > 0 */
    if (expectedArgs > 0) {
        *set1 = getSetByName(trimWhitespace(args[0]), &undefinedSet);
        if (*set1 == NULL) {
            /* Print an error message if the set name is undefined */
            printf("Undefined set name: %s\n", undefinedSet);
            return 0; /* Return 0 to indicate failure */
        }
    }

    /* Validate the second set if expectedArgs > 1 */
    if (expectedArgs > 1) {
        *set2 = getSetByName(trimWhitespace(args[1]), &undefinedSet);
        if (*set2 == NULL) {
            /* Print an error message if the set name is undefined */
            printf("Undefined set name: %s\n", undefinedSet);
            return 0; /* Return 0 to indicate failure */
        }
    }

    /* Validate the result set if expectedArgs > 2 */
    if (expectedArgs > 2) {
        *result = getSetByName(trimWhitespace(args[2]), &undefinedSet);
        if (*result == NULL) {
            /* Print an error message if the set name is undefined */
            printf("Undefined set name: %s\n", undefinedSet);
            return 0; /* Return 0 to indicate failure */
        }
    }

    return 1; /* Return 1 to indicate success */
}

void parseAndExecuteCommand(char* input) {
    char normalizedInput[256];
    int j = 0;
    int i;
    char* p;
    char* tokens[20];
    int num_tokens = 0;
    char* token;
    int k;

    /* Print the user input */
    printf("\nCommand: %s\n", input);

    for (i = 0; input[i] != '\0'; i++) {
        if (input[i] == ',' && (i == 0 || input[i - 1] == ' ' || input[i - 1] == '\t')) {
            normalizedInput[j++] = ',';  /* Handle leading commas */ 
        } else if (input[i] == ',' && input[i + 1] == ' ') {
            normalizedInput[j++] = ',';  /* Handle trailing commas */ 
        } else if (input[i] == ' ' || input[i] == '\t') {
            if (j > 0 && normalizedInput[j - 1] != ' ' && normalizedInput[j - 1] != ',') {
                normalizedInput[j++] = ' ';  /* Handle multiple spaces/tabs */ 
            }
        } else {
            normalizedInput[j++] = input[i];
        }
    }
    normalizedInput[j] = '\0';

    /* Check for multiple consecutive commas */
    p = strstr(normalizedInput, ",,");
    if (p != NULL) {
        printf("Multiple consecutive commas\n");
        return;
    }

    /* Tokenize the input string */
    token = strtok(normalizedInput, " ,");
    while (token != NULL && num_tokens < 20) {
        tokens[num_tokens++] = token;
        token = strtok(NULL, " ,");
    }
    tokens[num_tokens] = NULL; /* Null-terminate the tokens array */

    if (num_tokens == 0) {
        printf("No command provided\n");
        return;
    }

    /* Find and execute the command */
    for (i = 0; commands[i].name != NULL; i++) {
        if (strcmp(tokens[0], commands[i].name) == 0) {
            /* Check if there are missing parameters */
            if (commands[i].expected_args != -1 && num_tokens - 1 < commands[i].expected_args) {
                printf("Missing parameter\n");
                return;
            }
            /* Check if there are extra parameters */
            if (commands[i].expected_args != -1 && num_tokens - 1 > commands[i].expected_args) {
                printf("Extra text after end of command\n");
                return;
            }

            /* Check for missing commas in the original input string */
            if (commands[i].expected_args > 1) {
                int commas_needed = commands[i].expected_args - 1;
                int commas_found = 0;
                for (k = 0; input[k] != '\0'; k++) {
                    if (input[k] == ',') {
                        commas_found++;
                    }
                }
                if (commas_found < commas_needed) {
                    printf("Missing comma\n");
                    return;
                }
                if (commas_found > commas_needed) {
                    printf("Illegal comma\n");
                    return;
                }
            }

            /* Execute the command function */
            commands[i].func(tokens + 1, num_tokens - 1); /* Pass the arguments to the command function */
            return;
        }
    }

    /* Command not found */
    printf("Undefined command name: %s\n", tokens[0]);
}

/* Function to turn on a bit in a set */
void cmd_turnOn(char* args[], int num_args) {
    Set* set;
    char* undefinedSet = NULL;
    int bit;

    /* Get the set by its name */
    set = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    /* Convert the argument to integer and turn on the bit */
    bit = strtol(trimWhitespace(args[1]), NULL, 10);
    turnOn(*set, bit);
}

/* Function to print the bits that are on in a set */
void cmd_print_set(char* args[], int num_args) {
    Set* set;
    char* undefinedSet = NULL;

    /* Get the set by its name */
    set = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    /* Print the set name and the bits that are on */
    printf("%s: ", trimWhitespace(args[0]));
    print_set(*set);
}

/* Function to perform a union of two sets and store the result in a third set */
void cmd_union_set(char* args[], int num_args) {
    Set* set1;
    Set* set2;
    Set* result;

    if (getAndValidateSets(args, &set1, &set2, &result, 3)) {
        union_set(*result, *set1, *set2);
    }
}

/* Function to perform an intersection of two sets and store the result in a third set */
void cmd_intersect_set(char* args[], int num_args) {
    Set* set1;
    Set* set2;
    Set* result;

    if (getAndValidateSets(args, &set1, &set2, &result, 3)) {
        intersect_set(*result, *set1, *set2);
    }
}

/* Function to subtract one set from another and store the result in a third set */
void cmd_sub_set(char* args[], int num_args) {
    Set* set1;
    Set* set2;
    Set* result;

    if (getAndValidateSets(args, &set1, &set2, &result, 3)) {
        sub_set(*result, *set1, *set2);
    }
}

/* Function to perform a symmetric difference of two sets and store the result in a third set */
void cmd_symdiff_set(char* args[], int num_args) {
    Set* set1;
    Set* set2;
    Set* result;

    if (getAndValidateSets(args, &set1, &set2, &result, 3)) {
        symdiff_set(*result, *set1, *set2);
    }
}

/* Function to stop the program */
void cmd_stop(char* args[], int num_args) {
    running = 0;  /* Set the running flag to 0 to stop the loop */
}

int main(int argc, char *argv[]) {
    FILE *inputFile = NULL;
    int stopCommandGiven = 0;
    char input[256];

    /* Initialize sets to 0 */
    memset(SETA, 0, sizeof(Set));
    memset(SETB, 0, sizeof(Set));
    memset(SETC, 0, sizeof(Set));
    memset(SETD, 0, sizeof(Set));
    memset(SETE, 0, sizeof(Set));
    memset(SETF, 0, sizeof(Set));

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
        /* Check for trailing comma */
        if (input[strlen(input) - 1] == ',') {
            printf("Extra comma after end of command\n");
        } else {
            parseAndExecuteCommand(input);
        }
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
