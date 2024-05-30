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
void turnOff(Set set, int bit);
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
void cmd_turnOff(char* args[], int num_args);
void cmd_print_set(char* args[], int num_args);
void cmd_union_set(char* args[], int num_args);
void cmd_intersect_set(char* args[], int num_args);
void cmd_sub_set(char* args[], int num_args);
void cmd_symdiff_set(char* args[], int num_args);
void cmd_stop(char* args[], int num_args);
void parseAndExecuteCommand(char* input);

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
    {"turnOff", cmd_turnOff, 2},
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

/* Function to turn off a bit in a given set */
void turnOff(Set set, int bit) {
    set[bit / 8] &= ~(1 << (bit % 8));
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

    if (*str == 0)  /* All spaces? */
        return str;

    /* Trim trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;

    /* Write new null terminator character */
    end[1] = '\0';

    return str;
}

/* Function to parse user input and call the appropriate command */
void parseAndExecuteCommand(char* input) {
    /* Normalize the input by removing extra spaces and handling commas properly */
    char normalizedInput[256];
    int j = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ',' && (i == 0 || input[i - 1] == ' ' || input[i - 1] == '\t')) {
            normalizedInput[j++] = ',';
        } else if (input[i] == ',' && input[i + 1] == ' ') {
            normalizedInput[j++] = ',';
        } else if (input[i] == ' ' || input[i] == '\t') {
            if (j > 0 && normalizedInput[j - 1] != ' ' && normalizedInput[j - 1] != ',') {
                normalizedInput[j++] = ' ';
            }
        } else {
            normalizedInput[j++] = input[i];
        }
    }
    normalizedInput[j] = '\0';

    /* Check for multiple consecutive commas */
    char* p = strstr(normalizedInput, ",,");
    if (p != NULL) {
        printf("Error: Multiple consecutive commas\n");
        return;
    }

    char* tokens[20];
    int num_tokens = 0;
    char* token;

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
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(tokens[0], commands[i].name) == 0) {
            if (commands[i].expected_args != -1 && num_tokens - 1 < commands[i].expected_args) {
                printf("Missing parameter\n");
                return;
            }
            if (commands[i].expected_args != -1 && num_tokens - 1 > commands[i].expected_args) {
                printf("Error: Extra text after end of command\n");
                return;
            }
            /* Check for empty arguments (which result from consecutive commas) */
            for (int k = 1; k < num_tokens; k++) {
                if (strlen(trimWhitespace(tokens[k])) == 0) {
                    printf("Error: Multiple consecutive commas\n");
                    return;
                }
            }
            commands[i].func(tokens + 1, num_tokens - 1); /* Pass the arguments to the command function */
            return;
        }
    }

    printf("Undefined command name: %s\n", tokens[0]);
}

/* Function to read and set bits in a set */
void read_set(char* args[], int num_args) {
    Set* set;
    int bit;
    int end_with_minus_one;
    int i, j;
    char* undefinedSet = NULL;

    /* Check for multiple consecutive commas */
    for (i = 1; i < num_args; i++) {
        if (strlen(trimWhitespace(args[i])) == 0) {
            printf("Error: Multiple consecutive commas\n");
            return;
        }
    }

    set = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    end_with_minus_one = 0;
    for (i = 1; i < num_args; i++) {
        char* trimmedArg = trimWhitespace(args[i]);
        /* Convert argument to integer */
        char *endptr;
        bit = strtol(trimmedArg, &endptr, 10);
        /* Check for valid integer and within range */
        if (*endptr != '\0' || endptr == trimmedArg) {
            printf("Invalid set member %s - not an integer\n", trimmedArg);
            return;
        }
        if (bit == -1) {
            end_with_minus_one = 1;
            /* Check for any non-whitespace characters after -1 */
            for (j = i + 1; j < num_args; j++) {
                if (strlen(trimWhitespace(args[j])) != 0) {
                    printf("Error: Extra text after end of command\n");
                    return;
                }
            }
            break;  /* End of input */
        }
        if (bit < 0 || bit > 127) {
            printf("Invalid set member %d - value is out of range\n", bit);
            return;
        }
        turnOn(*set, bit);
    }
    if (!end_with_minus_one) {
        printf("List of set members is not terminated correctly (missing -1 value)\n");
        return;
    }
}

/* Command function implementations */
void cmd_turnOn(char* args[], int num_args) {
    Set* set;
    int bit;
    char* undefinedSet = NULL;

    set = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    bit = strtol(trimWhitespace(args[1]), NULL, 10);
    turnOn(*set, bit);
}

void cmd_turnOff(char* args[], int num_args) {
    Set* set;
    int bit;
    char* undefinedSet = NULL;

    set = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    bit = strtol(trimWhitespace(args[1]), NULL, 10);
    turnOff(*set, bit);
}

void cmd_print_set(char* args[], int num_args) {
    Set* set;
    char* undefinedSet = NULL;

    set = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    printf("%s: ", trimWhitespace(args[0]));
    print_set(*set);
}

void cmd_union_set(char* args[], int num_args) {
    Set* set1;
    Set* set2;
    Set* result;
    char* undefinedSet = NULL;

    set1 = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set1 == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }
    set2 = getSetByName(trimWhitespace(args[1]), &undefinedSet);
    if (set2 == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }
    result = getSetByName(trimWhitespace(args[2]), &undefinedSet);
    if (result == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    union_set(*result, *set1, *set2);
}

void cmd_intersect_set(char* args[], int num_args) {
    Set* set1;
    Set* set2;
    Set* result;
    char* undefinedSet = NULL;

    set1 = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set1 == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }
    set2 = getSetByName(trimWhitespace(args[1]), &undefinedSet);
    if (set2 == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }
    result = getSetByName(trimWhitespace(args[2]), &undefinedSet);
    if (result == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    intersect_set(*result, *set1, *set2);
}

void cmd_sub_set(char* args[], int num_args) {
    Set* set1;
    Set* set2;
    Set* result;
    char* undefinedSet = NULL;

    set1 = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set1 == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }
    set2 = getSetByName(trimWhitespace(args[1]), &undefinedSet);
    if (set2 == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }
    result = getSetByName(trimWhitespace(args[2]), &undefinedSet);
    if (result == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    sub_set(*result, *set1, *set2);
}

void cmd_symdiff_set(char* args[], int num_args) {
    Set* set1;
    Set* set2;
    Set* result;
    char* undefinedSet = NULL;

    set1 = getSetByName(trimWhitespace(args[0]), &undefinedSet);
    if (set1 == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }
    set2 = getSetByName(trimWhitespace(args[1]), &undefinedSet);
    if (set2 == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }
    result = getSetByName(trimWhitespace(args[2]), &undefinedSet);
    if (result == NULL) {
        printf("Undefined set name: %s\n", undefinedSet);
        return;
    }

    symdiff_set(*result, *set1, *set2);
}

void cmd_stop(char* args[], int num_args) {
    running = 0;  /* Set the running flag to 0 to stop the loop */
}

int main() {
    /* Initialize sets to 0 */
    memset(SETA, 0, sizeof(Set));
    memset(SETB, 0, sizeof(Set));
    memset(SETC, 0, sizeof(Set));
    memset(SETD, 0, sizeof(Set));
    memset(SETE, 0, sizeof(Set));
    memset(SETF, 0, sizeof(Set));

    /* Read commands from the user in a loop */
    char input[256];
    while (running) {
        printf("Enter a command > ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            /* Remove the newline character at the end if present */
            input[strcspn(input, "\n")] = '\0';
            
            /* Check for trailing comma */
            if (input[strlen(input) - 1] == ',') {
                printf("Error: Extra text after end of command\n");
            } else {
                parseAndExecuteCommand(input);
            }
        }
    }

    return 0;
}
