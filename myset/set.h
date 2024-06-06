#ifndef SET_H
#define SET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Define a new type for a set */
typedef char Set[16];

/* Command function prototypes */
typedef void (*CommandFunc)(char* args[], int num_args, Set sets[]);

/* Command structure */
typedef struct {
    char* name;
    CommandFunc func;
    int expected_args;
} Command;

/* Function prototypes */
void read_set(Set set, int bit);
int isOn(Set set, int bit);
void print_set(Set set);
void union_set(Set result, Set set1, Set set2);
void intersect_set(Set result, Set set1, Set set2);
void sub_set(Set result, Set set1, Set set2);
void symdiff_set(Set result, Set set1, Set set2);
Set* getSetByName(char* name, Set sets[], char** undefinedSet);
char* trimWhitespace(char* str);
void help_read_set(char* args[], int num_args, Set sets[]);
void cmd_print_set(char* args[], int num_args, Set sets[]);
void cmd_union_set(char* args[], int num_args, Set sets[]);
void cmd_intersect_set(char* args[], int num_args, Set sets[]);
void cmd_sub_set(char* args[], int num_args, Set sets[]);
void cmd_symdiff_set(char* args[], int num_args, Set sets[]);
void stop(char* args[], int num_args, int* running);
void parseAndExecuteCommand(char* input, Set sets[], int* running, Command commands[]);
int getAndValidateSets(char* args[], Set sets[], Set** set1, Set** set2, Set** result, int expectedArgs);

#endif
