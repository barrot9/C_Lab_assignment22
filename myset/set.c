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

int main() {
    // Initialize sets to 0
    memset(SETA, 0, sizeof(Set));
    memset(SETB, 0, sizeof(Set));
    memset(SETC, 0, sizeof(Set));
    memset(SETD, 0, sizeof(Set));
    memset(SETE, 0, sizeof(Set));
    memset(SETF, 0, sizeof(Set));

    char command[100];
    char setName1[10], setName2[10], setName3[10];
    int bit;

    while (1) {
        printf("Enter command (turnOn, turnOff, printSet, union, intersect, subtract, symdiff, quit): ");
        scanf("%s", command);

        if (strcmp(command, "turnOn") == 0) {
            scanf("%s %d", setName1, &bit);
            Set* set = getSetByName(setName1);
            if (set) turnOn(*set, bit);
            else printf("Invalid set name\n");
        } else if (strcmp(command, "turnOff") == 0) {
            scanf("%s %d", setName1, &bit);
            Set* set = getSetByName(setName1);
            if (set) turnOff(*set, bit);
            else printf("Invalid set name\n");
        } else if (strcmp(command, "printSet") == 0) {
            scanf("%s", setName1);
            Set* set = getSetByName(setName1);
            if (set) {
                printf("%s: ", setName1);
                printSet(*set);
            } else printf("Invalid set name\n");
        } else if (strcmp(command, "union") == 0) {
            scanf("%s %s %s", setName1, setName2, setName3);
            Set* set1 = getSetByName(setName1);
            Set* set2 = getSetByName(setName2);
            Set* result = getSetByName(setName3);
            if (set1 && set2 && result) unionSets(*result, *set1, *set2);
            else printf("Invalid set names\n");
        } else if (strcmp(command, "intersect") == 0) {
            scanf("%s %s %s", setName1, setName2, setName3);
            Set* set1 = getSetByName(setName1);
            Set* set2 = getSetByName(setName2);
            Set* result = getSetByName(setName3);
            if (set1 && set2 && result) intersectSets(*result, *set1, *set2);
            else printf("Invalid set names\n");
        } else if (strcmp(command, "subtract") == 0) {
            scanf("%s %s %s", setName1, setName2, setName3);
            Set* set1 = getSetByName(setName1);
            Set* set2 = getSetByName(setName2);
            Set* result = getSetByName(setName3);
            if (set1 && set2 && result) subtractSets(*result, *set1, *set2);
            else printf("Invalid set names\n");
        } else if (strcmp(command, "symdiff") == 0) {
            scanf("%s %s %s", setName1, setName2, setName3);
            Set* set1 = getSetByName(setName1);
            Set* set2 = getSetByName(setName2);
            Set* result = getSetByName(setName3);
            if (set1 && set2 && result) symDifferenceSets(*result, *set1, *set2);
            else printf("Invalid set names\n");
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Invalid command\n");
        }
    }

    return 0;
}
