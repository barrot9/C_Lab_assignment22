#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Define a new type for a set */
typedef char Set[16];

/* Define the six sets */
Set SETA, SETB, SETC, SETD, SETE, SETF;

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
void subtract_set(Set result, Set set1, Set set2) {
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
Set* getSetByName(char* name) {
    if (strcmp(name, "SETA") == 0) return &SETA;
    if (strcmp(name, "SETB") == 0) return &SETB;
    if (strcmp(name, "SETC") == 0) return &SETC;
    if (strcmp(name, "SETD") == 0) return &SETD;
    if (strcmp(name, "SETE") == 0) return &SETE;
    if (strcmp(name, "SETF") == 0) return &SETF;
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

/* Function to read and set bits in a set */
void read_set(char* args[]) {
    Set* set;
    int bit;
    int num_args;
    int end_with_minus_one;
    int i, j;

    set = getSetByName(trimWhitespace(args[0]));
    if (set == NULL) {
        printf("Undefined set name\n");
        return;
    }

    num_args = 0;  /* Track number of arguments processed */
    end_with_minus_one = 0;
    for (i = 1; args[i] != NULL; i++) {
        char* trimmedArg = trimWhitespace(args[i]);
        /* Check for empty argument */
        if (strlen(trimmedArg) == 0) {
            printf("Error: Empty argument\n");
            return;
        }
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
            for (j = i + 1; args[j] != NULL; j++) {
                if (strlen(trimWhitespace(args[j])) != 0) {
                    printf("Error: Extra text after ending value -1\n");
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
        num_args++;
    }
    /* Check for missing arguments */
    if (num_args == 0) {
        printf("Error: No bit values provided\n");
        return;
    }
    if (!end_with_minus_one) {
        printf("List of set members is not terminated correctly (missing -1 value)\n");
        return;
    }
}

/* Command function prototypes */
typedef void (*CommandFunc)(char* args[]);

void cmd_turnOn(char* args[]);
void cmd_turnOff(char* args[]);
void cmd_print_set(char* args[]);
void cmd_union_set(char* args[]);
void cmd_intersect_set(char* args[]);
void cmd_subtract_set(char* args[]);
void cmd_symdiff_set(char* args[]);
void cmd_stop(char* args[]);

/* Command structure */
typedef struct {
    char* name;
    CommandFunc func;
} Command;

Command commands[] = {
    {"turnOn", cmd_turnOn},
    {"turnOff", cmd_turnOff},
    {"print_set", cmd_print_set},
    {"union_set", cmd_union_set},
    {"intersect_set", cmd_intersect_set},
    {"subtract_set", cmd_subtract_set},
    {"symdiff_set", cmd_symdiff_set},
    {"read_set", read_set},
    {"stop", cmd_stop},
    {NULL, NULL}  /* End of commands marker */
};

int running = 1;  /* Global variable to control the loop */

/* Command function implementations */
void cmd_turnOn(char* args[]) {
    Set* set;
    int bit;
    
    set = getSetByName(trimWhitespace(args[0]));
    bit = strtol(trimWhitespace(args[1]), NULL, 10);
    if (set) turnOn(*set, bit);
    else printf("Undefined set name\n");
}

void cmd_turnOff(char* args[]) {
    Set* set;
    int bit;
    
    set = getSetByName(trimWhitespace(args[0]));
    bit = strtol(trimWhitespace(args[1]), NULL, 10);
    if (set) turnOff(*set, bit);
    else printf("Undefined set name\n");
}

void cmd_print_set(char* args[]) {
    Set* set;
    
    set = getSetByName(trimWhitespace(args[0]));
    if (set) {
        if (args[1] != NULL && strlen(trimWhitespace(args[1])) > 0) {
            printf("Error: Extra text after end of command\n");
        } else {
            printf("%s: ", trimWhitespace(args[0]));
            print_set(*set);
        }
    } else printf("Undefined set name\n");
}

void cmd_union_set(char* args[]) {
    Set* set1;
    Set* set2;
    Set* result;
    
    set1 = getSetByName(trimWhitespace(args[0]));
    set2 = getSetByName(trimWhitespace(args[1]));
    result = getSetByName(trimWhitespace(args[2]));
    if (set1 && set2 && result) union_set(*result, *set1, *set2);
    else printf("Undefined set names\n");
}

void cmd_intersect_set(char* args[]) {
    Set* set1;
    Set* set2;
    Set* result;
    
    set1 = getSetByName(trimWhitespace(args[0]));
    set2 = getSetByName(trimWhitespace(args[1]));
    result = getSetByName(trimWhitespace(args[2]));
    if (set1 && set2 && result) intersect_set(*result, *set1, *set2);
    else printf("Undefined set names\n");
}

void cmd_subtract_set(char* args[]) {
    Set* set1;
    Set* set2;
    Set* result;
    
    set1 = getSetByName(trimWhitespace(args[0]));
    set2 = getSetByName(trimWhitespace(args[1]));
    result = getSetByName(trimWhitespace(args[2]));
    if (set1 && set2 && result) subtract_set(*result, *set1, *set2);
    else printf("Undefined set names\n");
}

void cmd_symdiff_set(char* args[]) {
    Set* set1;
    Set* set2;
    Set* result;
    
    set1 = getSetByName(trimWhitespace(args[0]));
    set2 = getSetByName(trimWhitespace(args[1]));
    result = getSetByName(trimWhitespace(args[2]));
    if (set1 && set2 && result) symdiff_set(*result, *set1, *set2);
    else printf("Undefined set names\n");
}

void cmd_stop(char* args[]) {
    running = 0;  /* Set the running flag to 0 to stop the loop */
}

/* Function to parse user input and call the appropriate command */
void parseAndExecuteCommand(char* input) {
    char* tokens[20];
    int num_tokens = 0;
    char* token;
    int i;

    token = strtok(input, " ,");
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
            commands[i].func(tokens + 1); /* Pass the arguments to the command function */
            return;
        }
    }

    printf("Undefined command name: %s\n", tokens[0]);
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
        printf("> ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            /* Remove the newline character at the end if present */
            input[strcspn(input, "\n")] = '\0';
            parseAndExecuteCommand(input);
        }
    }

    return 0;
}
