#include "set.h"

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
