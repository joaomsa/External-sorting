#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "exsort.c"
#define INPUT "test.txt"
#define OUTPUT "testS.txt"
#define ENTRYMAX 1
#define ENTRYLEN 70

int main(){
    FILE *input, *output;
    input = fopen (INPUT, "r");
    output = fopen (OUTPUT, "w");

    int roundNum;
    roundNum = file_rounds_split(input, ENTRYMAX, ENTRYLEN);

    /* intersperse */
    file_rounds_intersperse(output, roundNum, ENTRYLEN);

    /* Write to final file
     * Delete rounds */
    return 0;
}
