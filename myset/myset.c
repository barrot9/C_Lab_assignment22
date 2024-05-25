#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Define a new type for a set
typedef char Set[16];

// Define the six sets
Set SETA, SETB, SETC, SETD, SETE, SETF;

// Function to turn on a bit in a given set
void turnOn(Set set, int bit) {
    set[bit / 8] |= (1 << (bit % 8));
}

// Function to turn off a bit in a given set
void turnOff(Set set, int bit) {
    set[bit / 8] &= ~(1 << (bit % 8));
}

// Function to check if a bit is on in a given set
int isOn(Set set, int bit) {
    return set[bit / 8] & (1 << (bit % 8));
}

// Function to print the numbers of the bits that are on in a set
void printSet(Set set) {
    int first = 1;  // flag to manage comma
    for (int i = 0; i < 128; i++) {
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

// Function to perform a union of two sets and store the result in a third set
void unionSets(Set result, Set set1, Set set2) {
    for (int i = 0; i < 16; i++) {
        result[i] = set1[i] | set2[i];
    }
}

// Function to perform an intersection of two sets and store the result in a third set
void intersectSets(Set result, Set set1, Set set2) {
    for (int i = 0; i < 16; i++) {
        result[i] = set1[i] & set2[i];
    }
}

// Function to subtract set2 from set1 and store the result in a third set
void subtractSets(Set result, Set set1, Set set2) {
    for (int i = 0; i < 16; i++) {
        result[i] = set1[i] & ~set2[i];
    }
}

// Function to perform a symmetric difference of two sets and store the result in a third set
void symDifferenceSets(Set result, Set set1, Set set2) {
    for (int i = 0; i < 16; i++) {
        result[i] = set1[i] ^ set2[i];
    }
}

// Function to get a set by its name
Set* getSetByName(char* name) {
    if (strcmp(name, "SETA") == 0) return &SETA;
    if (strcmp(name, "SETB") == 0) return &SETB;
    if (strcmp(name, "SETC") == 0) return &SETC;
    if (strcmp(name, "SETD") == 0) return &SETD;
    if (strcmp(name, "SETE") == 0) return &SETE;
    if (strcmp(name, "SETF") == 0) return &SETF;
    return NULL;
}

// Function to trim leading and trailing whitespace
char* trimWhitespace(char* str) {
    // Trim leading whitespace
    while (isspace((unsigned char) *str)) str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing whitespace
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

// Function to read and set bits in a set
void read_set(char* args[]) {
    Set* set = getSetByName(trimWhitespace(args[0]));
    if (set == NULL) {
        printf("Invalid set name\n");
        return;
    }

    int bit;
    int num_args = 0; // Track number of arguments processed
    int end_with_minus_one = 0;
    for (int i = 1; args[i] != NULL; i++) {
        char* trimmedArg = trimWhitespace(args[i]);
        // Check for empty argument
        if (strlen(trimmedArg) == 0) {
            printf("Error: Extra comma\n");
            return;
        }
        // Convert argument to integer
        bit = atoi(trimmedArg);
        // Check for valid integer and within range
        if (bit == 0 && trimmedArg[0] != '0') {
            printf("Invalid bit value: %s\n", trimmedArg);
            return;
        }
        if (bit == -1) {
            end_with_minus_one = 1;
            break;  // End of input
        }
        if (bit < 0 || bit > 127) {
            printf("Invalid bit value: %d\n", bit);
            return;
        }
        turnOn(*set, bit);
        num_args++;
    }
    // Check for missing arguments
    if (num_args == 0) {
        printf("Error: No bit values provided\n");
        return;
    }
    if (!end_with_minus_one) {
        printf("Missing -1 value\n");
        return;
    }
}

// Command function prototypes
typedef void (*CommandFunc)(char* args[]);

void cmd_turnOn(char* args[]);
void cmd_turnOff(char* args[]);
void cmd_printSet(char* args[]);
void cmd_union(char* args[]);
void cmd_intersect(char* args[]);
void cmd_subtract(char* args[]);
void cmd_symdiff(char* args[]);
void cmd_exit(char* args[]);

// Command structure
typedef struct {
    char* name;
    CommandFunc func;
} Command;

Command commands[] = {
    {"turnOn", cmd_turnOn},
    {"turnOff", cmd_turnOff},
    {"printSet", cmd_printSet},
    {"union", cmd_union},
    {"intersect", cmd_intersect},
    {"subtract", cmd_subtract},
    {"symdiff", cmd_symdiff},
    {"read_set", read_set},
    {"exit", cmd_exit},
    {NULL, NULL}  // End of commands marker
};

int running = 1;  // Global variable to control the loop

// Command function implementations
void cmd_turnOn(char* args[]) {
    Set* set = getSetByName(trimWhitespace(args[0]));
    int bit = atoi(trimWhitespace(args[1]));
    if (set) turnOn(*set, bit);
    else printf("Invalid set name\n");
}

void cmd_turnOff(char* args[]) {
    Set* set = getSetByName(trimWhitespace(args[0]));
    int bit = atoi(trimWhitespace(args[1]));
    if (set) turnOff(*set, bit);
    else printf("Invalid set name\n");
}

void cmd_printSet(char* args[]) {
    Set* set = getSetByName(trimWhitespace(args[0]));
    if (set) {
        if (args[1] != NULL) {
            printf("Error: Extra text after end of command\n");
        } else {
            printf("%s: ", trimWhitespace(args[0]));
            printSet(*set);
        }
    } else printf("Invalid set name\n");
}

void cmd_union(char* args[]) {
    Set* set1 = getSetByName(trimWhitespace(args[0]));
    Set* set2 = getSetByName(trimWhitespace(args[1]));
    Set* result = getSetByName(trimWhitespace(args[2]));
    if (set1 && set2 && result) unionSets(*result, *set1, *set2);
    else printf("Invalid set names\n");
}

void cmd_intersect(char* args[]) {
    Set* set1 = getSetByName(trimWhitespace(args[0]));
    Set* set2 = getSetByName(trimWhitespace(args[1]));
    Set* result = getSetByName(trimWhitespace(args[2]));
    if (set1 && set2 && result) intersectSets(*result, *set1, *set2);
    else printf("Invalid set names\n");
}

void cmd_subtract(char* args[]) {
    Set* set1 = getSetByName(trimWhitespace(args[0]));
    Set* set2 = getSetByName(trimWhitespace(args[1]));
    Set* result = getSetByName(trimWhitespace(args[2]));
    if (set1 && set2 && result) subtractSets(*result, *set1, *set2);
    else printf("Invalid set names\n");
}

void cmd_symdiff(char* args[]) {
    Set* set1 = getSetByName(trimWhitespace(args[0]));
    Set* set2 = getSetByName(trimWhitespace(args[1]));
    Set* result = getSetByName(trimWhitespace(args[2]));
    if (set1 && set2 && result) symDifferenceSets(*result, *set1, *set2);
    else printf("Invalid set names\n");
}

void cmd_exit(char* args[]) {
    running = 0;  // Set the running flag to 0 to exit the loop
}

// Function to parse user input and call the appropriate command
void parseAndExecuteCommand(char* input) {
    char* tokens[20];
    int num_tokens = 0;

    // Trim leading and trailing whitespace from input
    input = trimWhitespace(input);

    // Split input into command and arguments
    char* cmd = strtok(input, " ");
    if (cmd != NULL) {
        tokens[num_tokens++] = cmd;
        if (strcmp(cmd, "read_set") == 0) {
            char* token = strtok(NULL, ",");
            while (token != NULL) {
                tokens[num_tokens++] = token;
                token = strtok(NULL, ",");
            }
        } else {
            char* token = strtok(NULL, " ");
            while (token != NULL) {
                tokens[num_tokens++] = token;
                token = strtok(NULL, " ");
            }
        }
    }
    tokens[num_tokens] = NULL;  // Null-terminate the tokens array

    if (num_tokens == 0) {
        printf("No command provided\n");
        return;
    }

    // Find and execute the command
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(tokens[0], commands[i].name) == 0) {
            // Ensure no extra text after the command for printSet
            if (strcmp(tokens[0], "printSet") == 0 && tokens[2] != NULL) {
                printf("Error: Extra text after end of command\n");
                return;
            }
            commands[i].func(tokens + 1);  // Pass the arguments to the command function
            return;
        }
    }

    printf("Unknown command: %s\n", tokens[0]);
}

int main() {
    // Initialize sets to 0
    memset(SETA, 0, sizeof(Set));
    memset(SETB, 0, sizeof(Set));
    memset(SETC, 0, sizeof(Set));
    memset(SETD, 0, sizeof(Set));
    memset(SETE, 0, sizeof(Set));
    memset(SETF, 0, sizeof(Set));

    // Read commands from the user in a loop
    char input[256];
    while (running) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Remove the newline character at the end if present
            input[strcspn(input, "\n")] = '\0';
            parseAndExecuteCommand(input);
        }
    }

    return 0;
}
