#ifndef SET_H
#define SET_H

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

int running = 1;  /* Global variable to control the loop in main */

#endif
