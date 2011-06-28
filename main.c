#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#ifndef EXSORT_H
#define EXSORT_H
#include "exsort.h"
#endif

#define INPUT argv[1]
#define OUTPUT argv[2]
#define ENTRYMAX argv[3]

int main(int argc, char *argv[]){
    FILE *input, *output;
    int roundNum;
    if (argc != 4)
        printf("Missing parameters\n");

    input = fopen(INPUT, "rb");
    output = fopen(OUTPUT, "w");

    /* Split and sort rounds */
    roundNum = file_rounds_split(input, strtol(ENTRYMAX, NULL, 10));

    /* Intersperse rounds and write to sorted output*/
    file_rounds_intersperse(output, roundNum);

    return 0;
}
