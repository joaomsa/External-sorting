#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "exsort.c"
#define INPUT "test.txt"
#define OUTPUT empty
#define ENTRIESMAX 2

int main(){
    FILE *unsortedInput;
    unsortedInput = fopen (INPUT, "r");
    assert(unsortedInput != NULL);

    unsigned long inputLen;
    char *unsortedInputStr;
    inputLen = file_len(unsortedInput);
    unsortedInputStr = (char*) malloc (inputLen * sizeof(char));
    fread(unsortedInputStr, inputLen, sizeof(char), unsortedInput);

    int entriesTotal
    entriesTotal = str_numchar(unsortedInputStr, '\n');

    /*
    char *url, *views;
    url = strtok(unsortedInputStr, " \n");
    views = strtok(NULL, " \n");
    printf("%s %i", url, atoi(views));
    */
    return 0;
}
