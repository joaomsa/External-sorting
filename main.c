#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "exsort.c"
#define INPUT "test.txt"
#define OUTPUT empty
#define ENTRIESMAX 4
#define ENTRYLEN 103

int main(){
    FILE *input;
    input = fopen (INPUT, "r");
    assert (input != NULL);

    int roundNum = file_rounds_split(input, ENTRIESMAX, ENTRYLEN);
    /* remove later */
    while(roundNum)
        roundNum = 0;

    /*
       int entriesTotal
       entriesTotal = str_numchar(unsortedInputStr, '\n');

       char *url, *views;
       url = strtok(unsortedInputStr, " \n");
       views = strtok(NULL, " \n");
       printf("%s %i", url, atoi(views));
       */

    /* intersperse 
     * Write to final file
     * Delete rounds */
    return 0;
}
