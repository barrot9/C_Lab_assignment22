#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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


// Function to read and set bits in a set
void read_set(char* args[]) {
    Set* set = getSetByName(args[0]);
    if (set == NULL) {
        printf("Invalid set name\n");
        return;
    }

    int bit;
    int num_args = 0; // Track number of arguments processed
    for (int i = 1; args[i] != NULL; i++) {
        // Check for empty argument
        if (strlen(args[i]) == 0) {
            printf("Error: Empty argument\n");
            return;
        }
        // Check for missing comma
        if (i == 1 && args[i][strlen(args[i]) - 1] != ',') {
            printf("Error: Missing comma after set name\n");
            return;
        }
        // Convert argument to integer
        bit = atoi(args[i]);
        // Check for extra comma
        if (args[i][0] == ',' && (args[i - 1] == NULL || args[i + 1] == NULL || strlen(args[i + 1]) == 0)) {
            printf("Error: Extra comma\n");
            return;
        }
        // Check for valid integer and within range
        if (bit == 0 && args[i][0] != '0') {
            printf("Invalid bit value: %s\n", args[i]);
            return;
        }
        if (bit == -1) break;  // End of input
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
    {NULL, NULL}  // End of commands marker
};

// Command function implementations
void cmd_turnOn(char* args[]) {
    Set* set = getSetByName(args[0]);
    int bit = atoi(args[1]);
    if (set) turnOn(*set, bit);
    else printf("Invalid set name\n");
}

void cmd_turnOff(char* args[]) {
    Set* set = getSetByName(args[0]);
    int bit = atoi(args[1]);
    if (set) turnOff(*set, bit);
    else printf("Invalid set name\n");
}

void cmd_printSet(char* args[]) {
    Set* set = getSetByName(args[0]);
    if (set) {
        printf("%s: ", args[0]);
        printSet(*set);
    } else printf("Invalid set name\n");
}

void cmd_union(char* args[]) {
    Set* set1 = getSetByName(args[0]);
    Set* set2 = getSetByName(args[1]);
    Set* result = getSetByName(args[2]);
    if (set1 && set2 && result) unionSets(*result, *set1, *set2);
    else printf("Invalid set names\n");
}

void cmd_intersect(char* args[]) {
    Set* set1 = getSetByName(args[0]);
    Set* set2 = getSetByName(args[1]);
    Set* result = getSetByName(args[2]);
    if (set1 && set2 && result) intersectSets(*result, *set1, *set2);
    else printf("Invalid set names\n");
}

void cmd_subtract(char* args[]) {
    Set* set1 = getSetByName(args[0]);
    Set* set2 = getSetByName(args[1]);
    Set* result = getSetByName(args[2]);
    if (set1 && set2 && result) subtractSets(*result, *set1, *set2);
    else printf("Invalid set names\n");
}

void cmd_symdiff(char* args[]) {
    Set* set1 = getSetByName(args[0]);
    Set* set2 = getSetByName(args[1]);
    Set* result = getSetByName(args[2]);
    if (set1 && set2 && result) symDifferenceSets(*result, *set1, *set2);
    else printf("Invalid set names\n");
}

int main() {
    // Initialize sets to 0
    memset(SETA, 0, sizeof(Set));
    memset(SETB, 0, sizeof(Set));
    memset(SETC, 0, sizeof(Set));
    memset(SETD, 0, sizeof(Set));
    memset(SETE, 0, sizeof(Set));
    memset(SETF, 0, sizeof(Set));

    // Test cases for read_set function
    char* test_cases[] = {
        "read_set SETA 3, 4, 5, 76, 1, -1",  // Normal case
        "read_set SETB 128, -1",              // Out of range bit value
        "read_set SETC 3, 4,, 5, 76, 1, -1",  // Extra comma
        "read_set SETD 3.14, -1",              // Non-integer bit value
        "read_set SETE 3, abc, 5, 76, 1, -1", // Invalid bit value
        "read_set SETF, -1",                   // Missing bit values
        NULL
    };

    // Test read_set with test cases
    printf("Testing read_set function:\n");
    for (int i = 0; test_cases[i] != NULL; i++) {
        printf("Test case %d: %s\n", i + 1, test_cases[i]);
        parseAndExecuteCommand(test_cases[i]);
        printf("\n");
    }

    // Test other commands
    printf("Testing other commands:\n");
    parseAndExecuteCommand("printSet SETA");
    parseAndExecuteCommand("printSet SETB");
    parseAndExecuteCommand("printSet SETC");
    parseAndExecuteCommand("printSet SETD");
    parseAndExecuteCommand("printSet SETE");
    parseAndExecuteCommand("printSet SETF");

    return 0;
}


