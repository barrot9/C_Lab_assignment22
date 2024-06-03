#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Define a new type for a set */
typedef char Set[16];

/* Define the six sets */
Set SETA, SETB, SETC, SETD, SETE, SETF;

/* Function prototypes */
void turnOn(Set set, int bit);
int isOn(Set set, int bit);
void print_set(Set set);
void union_set(Set result, Set set1, Set set2);
void intersect_set(Set result, Set set1, Set set2);
void sub_set(Set result, Set set1, Set set2);
void symdiff_set(Set result, Set set1, Set set2);
Set* getSetByName(char* name, char** undefinedSet);
char* trimWhitespace(char* str);
void read_set(char* args[], int num_args);
void cmd_turnOn(char* args[], int num_args);
void cmd_print_set(char* args[], int num_args);
void cmd_union_set(char* args[], int num_args);
void cmd_intersect_set(char* args[], int num_args);
void cmd_sub_set(char* args[], int num_args);
void cmd_symdiff_set(char* args[], int num_args);
void cmd_stop(char* args[], int num_args);
void parseAndExecuteCommand(char* input);
int getAndValidateSets(char* args[], Set** set1, Set** set2, Set** result, int expectedArgs);

/* Command function prototypes */
typedef void (*CommandFunc)(char* args[], int num_args);

/* Command structure */
typedef struct {
    char* name;
    CommandFunc func;
    int expected_args;
} Command;

/* Commands array */
Command commands[] = {
    {"turnOn", cmd_turnOn, 2},
    {"print_set", cmd_print_set, 1},
    {"union_set", cmd_union_set, 3},
    {"intersect_set", cmd_intersect_set, 3},
    {"sub_set", cmd_sub_set, 3},
    {"symdiff_set", cmd_symdiff_set, 3},
    {"read_set", read_set, -1},  /* -1 indicates variable number of arguments */
    {"stop", cmd_stop, 0},
    {NULL, NULL, 0}  /* End of commands marker */
};

int running = 1;  /* Global variable to control the loop */

/* Function to turn on a bit in a given set */
void turnOn(Set set, int bit) {
    set[bit / 8] |= (1 << (bit % 8));
}

/* Function to check if a bit is on in a given set */
int isOn(Set set, int bit) {
    return set[bit / 8] & (1 << (bit % 8));
}

/* Function to print the numbers of the bits that are on in a set */
void print_set(Set set) {
    int first = 1;  /* flag to manage comma */
    int i;
    for (i = 0; i < 128; i++) {
        if (isOn(set, i)) {
            if (!first) {
                printf(", ");
            }
            printf("%d", i);
            first = 0;
        }
    }
    printf("\n");
}

/* Function to perform a union of two sets and store the result in a third set */
void union_set(Set result, Set set1, Set set2) {
    int i;
    for (i = 0; i < 16; i++) {
        result[i] = set1[i] | set2[i];
    }
}

/* Function to perform an intersection of two sets and store the result in a third set */
void intersect_set(Set result, Set set1, Set set2) {
    int i;
    for (i = 0; i < 16; i++) {
        result[i] = set1[i] & set2[i];
    }
}

/* Function to subtract set2 from set1 and store the result in a third set */
void sub_set(Set result, Set set1, Set set2) {
    int i;
    for (i = 0; i < 16; i++) {
        result[i] = set1[i] & ~set2[i];
    }
}

/* Function to perform a symmetric difference of two sets and store the result in a third set */
void symdiff_set(Set result, Set set1, Set set2) {
    int i;
    for (i = 0; i < 16; i++) {
        result[i] = set1[i] ^ set2[i];
    }
}

/* Function to get a set by its name */
Set* getSetByName(char* name, char** undefinedSet) {
    if (strcmp(name, "SETA") == 0) return &SETA;
    if (strcmp(name, "SETB") == 0) return &SETB;
    if (strcmp(name, "SETC") == 0) return &SETC;
    if (strcmp(name, "SETD") == 0) return &SETD;
    if (strcmp(name, "SETE") == 0) return &SETE;
    if (strcmp(name, "SETF") == 0) return &SETF;
    *undefinedSet = name;
    return NULL;
}

/* Function to trim leading and trailing whitespace */
char* trimWhitespace(char* str) {
    char *end;
    /* Trim leading whitespace */
    while (isspace((unsigned char) *str)) str++;

    if (*str == 0)  /* check if command is empty or all spaces */
        return str;

    /* Trim trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;

    /* Write new null terminator character */
    end[1] = '\0';

    return str;
}

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

/* Function to read and set bits in a set */
void read_set(char* args[], int num_args) {
    Set* set;
    int bit;
    int end_with_minus_one;
    int i, j;
    char* undefinedSet = NULL;
    char* trimmedArg;
    char* endptr;

    /* Get the set by its name */
    set = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    /* Check if set members are missing */
    if (num_args < 2) {
        printf("Missing set members\n");
        return;
    }

    /* Initialize the set to 0 */
    memset(*set, 0, sizeof(Set));

    /* Check for multiple consecutive commas */
    for (i = 1; i < num_args; i++) {
        if (strlen(trimWhitespace(args[i])) == 0) {
            memset(*set, 0, sizeof(Set));
            printf("Multiple consecutive commas\n");
            return;
        }
    }

    end_with_minus_one = 0;
    for (i = 1; i < num_args; i++) {
        trimmedArg = trimWhitespace(args[i]);
        /* Convert argument to integer */
        bit = strtol(trimmedArg, &endptr, 10);
        /* Check for valid integer and within range */
        if (*endptr != '\0' || endptr == trimmedArg) {
            memset(*set, 0, sizeof(Set));
            printf("Invalid set member %s - not an integer\n", trimmedArg);
            return;
        }
        /* Check for end of input (-1) */
        if (bit == -1) {
            end_with_minus_one = 1;
            /* Check for any non-whitespace characters after -1 */
            for (j = i + 1; j < num_args; j++) {
                if (strlen(trimWhitespace(args[j])) != 0) {
                    memset(*set, 0, sizeof(Set));
                    printf("Extra text after end of command\n");
                    return;
                }
            }
            break;  /* End of input */
        }
        /* Check for bit value out of range */
        if (bit < 0 || bit > 127) {
            memset(*set, 0, sizeof(Set));
            printf("Invalid set member %d - value is out of range\n", bit);
            return;
        }
        /* Turn on the bit in the set */
        turnOn(*set, bit);
    }
    /* Check if input was correctly terminated with -1 */
    if (!end_with_minus_one) {
        memset(*set, 0, sizeof(Set));
        printf("List of set members is not terminated correctly (missing -1 value)\n");
        return;
    }
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
