#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "exsort.c"
#define INPUT "test.txt"
#define OUTPUT empty
#define ENTRIESMAX 1
#define ENTRYLEN 103

int main(){
    FILE *input;
    input = fopen (INPUT, "r");
    assert (input != NULL);

    int roundNum = file_rounds_split(input, ENTRIESMAX, ENTRYLEN);
    /* remove later */
    while(roundNum)
        roundNum = 0;

    /* intersperse 
     * Write to final file
     * Delete rounds */
    return 0;
}
