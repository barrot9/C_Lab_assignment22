#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> /* Include stdlib.h for strtod function */ 

/* Define the type for set */
typedef unsigned long long int set;

/* Constants */
#define MAX_NUMBERS 128
#define MAX_SETS 6
#define MAX_LINE_LENGTH 100

/* Define sets */
set SETA = 0, SETB = 0, SETC = 0, SETD = 0, SETE = 0, SETF = 0;
set *sets[MAX_SETS] = {&SETA, &SETB, &SETC, &SETD, &SETE, &SETF};

/* Function prototypes */
void read_set(char name, char numbers[]);
void print_set(char name);
void union_set(char names[], char result_name);
void intersect_set(char names[], char result_name);
void sub_set(char names[], char result_name);
void symdiff_set(char names[], char result_name);

int main() {
    char command[MAX_LINE_LENGTH];
    while (true) {
        fgets(command, MAX_LINE_LENGTH, stdin);

        /* Remove leading and trailing whitespace */
        char trimmed_command[MAX_LINE_LENGTH];
        sscanf(command, " %[^\n]s", trimmed_command);

        /* Parse command */
        char operation[20], set_names[MAX_LINE_LENGTH], result_set, extra;
        char numbers[MAX_LINE_LENGTH]; /* Changed to char array to read non-integer characters */ 
        if (sscanf(trimmed_command, "%s", operation) == EOF) {
            continue;
        }

        if (strcmp(operation, "stop") == 0) {
            if (sscanf(trimmed_command, "%*s %c", &extra) != EOF) {
                printf("Extraneous text after end of command\n");
                continue;
            }
            break; /* stop the program */
        }
        else if (strcmp(operation, "read_set") == 0) {
            if (sscanf(trimmed_command, "%*s %c %[^\n]", &set_names[0], numbers) != 2 || set_names[0] < 'A' || set_names[0] > 'F') {
                printf("undefined set name\n");
                continue;
            }
            if (sscanf(trimmed_command, "%*s %c %c", &extra, &extra) != EOF) {
                printf("Extraneous text after end of command\n");
                continue;
            }
            read_set(set_names[0], numbers); /* read set */
        }
        else if (strcmp(operation, "print_set") == 0) {
            if (sscanf(trimmed_command, "%*s %c", &set_names[0]) != 1 || set_names[0] < 'A' || set_names[0] > 'F') {
                printf("undefined set name\n");
                continue;
            }
            if (sscanf(trimmed_command, "%*s %c %c", &extra, &extra) != EOF) {
                printf("Extraneous text after end of command\n");
                continue;
            }
            print_set(set_names[0]); /* print set */
        }
        else if (strcmp(operation, "union_set") == 0) {
            if (sscanf(trimmed_command, "%*s %[^\n] %c", set_names, &result_set) != 2) {
                printf("Missing parameter\n");
                continue;
            }
            if (strstr(set_names, ",,")) {
                printf("multiple consecutive commas\n");
                continue;
            }
            if (strstr(set_names, ",") == NULL) {
                printf("missing comma\n");
                continue;
            }
            if (set_names[0] == ',') {
                printf("illegal comma\n");
                continue;
            }
            if (sscanf(trimmed_command, "%*s %[^\n] %c %c", set_names, &result_set, &extra) != EOF) {
                printf("Extraneous text after end of command\n");
                continue;
            }
            union_set(set_names, result_set); /* union sets */
        }
        else if (strcmp(operation, "intersect_set") == 0) {
            if (sscanf(trimmed_command, "%*s %[^\n] %c", set_names, &result_set) != 2) {
                printf("Missing parameter\n");
                continue;
            }
            if (strstr(set_names, ",,")) {
                printf("multiple consecutive commas\n");
                continue;
            }
            if (strstr(set_names, ",") == NULL) {
                printf("missing comma\n");
                continue;
            }
            if (set_names[0] == ',') {
                printf("illegal comma\n");
                continue;
            }
            if (sscanf(trimmed_command, "%*s %[^\n] %c %c", set_names, &result_set, &extra) != EOF) {
                printf("Extraneous text after end of command\n");
                continue;
            }
            intersect_set(set_names, result_set); /* intersect sets */
        }
        else if (strcmp(operation, "sub_set") == 0) {
            if (sscanf(trimmed_command, "%*s %[^\n] %c", set_names, &result_set) != 2) {
                printf("Missing parameter\n");
                continue;
            }
            if (strstr(set_names, ",,")) {
                printf("multiple consecutive commas\n");
                continue;
            }
            if (strstr(set_names, ",") == NULL) {
                printf("missing comma\n");
                continue;
            }
            if (set_names[0] == ',') {
                printf("illegal comma\n");
                continue;
            }
            if (sscanf(trimmed_command, "%*s %[^\n] %c %c", set_names, &result_set, &extra) != EOF) {
                printf("Extraneous text after end of command\n");
                continue;
            }
            sub_set(set_names, result_set); /* subtract sets */
        }
        else if (strcmp(operation, "symdiff_set") == 0) {
            if (sscanf(trimmed_command, "%*s %[^\n] %c", set_names, &result_set) != 2) {
                printf("Missing parameter\n");
                continue;
            }
            if (strstr(set_names, ",,")) {
                printf("multiple consecutive commas\n");
                continue;
            }
            if (strstr(set_names, ",") == NULL) {
                printf("missing comma\n");
                continue;
            }
            if (set_names[0] == ',') {
                printf("illegal comma\n");
                continue;
            }
            if (sscanf(trimmed_command, "%*s %[^\n] %c %c", set_names, &result_set, &extra) != EOF) {
                printf("Extraneous text after end of command\n");
                continue;
            }
            symdiff_set(set_names, result_set); /* symmetric difference sets */
        }
        else {
            printf("undefined command\n");
        }
    }
    return 0;
}

void read_set(char name, char numbers[]) {
    set *target_set;
    switch (name) {
        case 'A':
            target_set = &SETA;
            break;
        case 'B':
            target_set = &SETB;
            break;
        case 'C':
            target_set = &SETC;
            break;
        case 'D':
            target_set = &SETD;
            break;
        case 'E':
            target_set = &SETE;
            break;
        case 'F':
            target_set = &SETF;
            break;
        default:
            return;
    }

    *target_set = 0;
    int i = 0;
    char *token = strtok(numbers, ",");
    while (token != NULL) {
        double num = strtod(token, NULL); /* Convert token to double */ 
        if (num == 0 && token[0] != '0') { /* strtod returns 0 for non-numeric strings */ 
            printf("invalid set member - not a number\n");
            return;
        }
        if (num != (int)num) {
            printf("invalid set member - not an integer\n");
            return;
        }
        if (num < 0 || num > 127) {
            printf("invalid set member - value out of range\n");
            return;
        }
        *target_set |= 1ull << (int)num;
        token = strtok(NULL, ",");
    }
}

void print_set(char name) {
    set *target_set;
    switch (name) {
        case 'A':
            target_set = &SETA;
            break;
        case 'B':
            target_set = &SETB;
            break;
        case 'C':
            target_set = &SETC;
            break;
        case 'D':
            target_set = &SETD;
            break;
        case 'E':
            target_set = &SETE;
            break;
        case 'F':
            target_set = &SETF;
            break;
        default:
            return;
    }

    printf("Set %c:", name);
    bool empty = true;
    for (int i = 0; i < 128; i++) {
        if (*target_set & (1ull << i)) {
            printf(" %d", i);
            empty = false;
        }
    }
    if (empty) {
        printf(" (empty)");
    }
    printf("\n");
}

void union_set(char names[], char result_name) {
    set *result_set;
    switch (result_name) {
        case 'A':
            result_set = &SETA;
            break;
        case 'B':
            result_set = &SETB;
            break;
        case 'C':
            result_set = &SETC;
            break;
        case 'D':
            result_set = &SETD;
            break;
        case 'E':
            result_set = &SETE;
            break;
        case 'F':
            result_set = &SETF;
            break;
        default:
            return;
    }

    *result_set = 0;

    char *token;
    token = strtok(names, ",");
    while (token != NULL) {
        char set_name = token[1];
        if (set_name >= 'A' && set_name <= 'F') {
            set *set_to_union = sets[set_name - 'A'];
            *result_set |= *set_to_union;
        }
        else {
            printf("undefined set name\n");
            return;
        }
        token = strtok(NULL, ",");
    }
}

void intersect_set(char names[], char result_name) {
    set *result_set;
    switch (result_name) {
        case 'A':
            result_set = &SETA;
            break;
        case 'B':
            result_set = &SETB;
            break;
        case 'C':
            result_set = &SETC;
            break;
        case 'D':
            result_set = &SETD;
            break;
        case 'E':
            result_set = &SETE;
            break;
        case 'F':
            result_set = &SETF;
            break;
        default:
            return;
    }

    *result_set = sets[names[0] - 'A'][0];
    char *token = strtok(names + 2, ",");
    while (token != NULL) {
        char set_name = token[1];
        if (set_name >= 'A' && set_name <= 'F') {
            *result_set &= sets[set_name - 'A'][0];
        }
        else {
            printf("undefined set name\n");
            return;
        }
        token = strtok(NULL, ",");
    }
}

void sub_set(char names[], char result_name) {
    set *result_set;
    switch (result_name) {
        case 'A':
            result_set = &SETA;
            break;
        case 'B':
            result_set = &SETB;
            break;
        case 'C':
            result_set = &SETC;
            break;
        case 'D':
            result_set = &SETD;
            break;
        case 'E':
            result_set = &SETE;
            break;
        case 'F':
            result_set = &SETF;
            break;
        default:
            return;
    }

    *result_set = sets[names[0] - 'A'][0];
    char *token = strtok(names + 2, ",");
    while (token != NULL) {
        char set_name = token[1];
        if (set_name >= 'A' && set_name <= 'F') {
            *result_set &= ~sets[set_name - 'A'][0];
        }
        else {
            printf("undefined set name\n");
            return;
        }
        token = strtok(NULL, ",");
    }
}

void symdiff_set(char names[], char result_name) {
    set *result_set;
    switch (result_name) {
        case 'A':
            result_set = &SETA;
            break;
        case 'B':
            result_set = &SETB;
            break;
        case 'C':
            result_set = &SETC;
            break;
        case 'D':
            result_set = &SETD;
            break;
        case 'E':
            result_set = &SETE;
            break;
        case 'F':
            result_set = &SETF;
            break;
        default:
            return;
    }

    *result_set = sets[names[0] - 'A'][0];
    char *token = strtok(names + 2, ",");
    while (token != NULL) {
        char set_name = token[1];
        if (set_name >= 'A' && set_name <= 'F') {
            *result_set ^= sets[set_name - 'A'][0];
        }
        else {
            printf("undefined set name\n");
            return;
        }
        token = strtok(NULL, ",");
    }
}
