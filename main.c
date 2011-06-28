#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    output = fopen(OUTPUT, "wb");

    /* Split and sort rounds */
    roundNum = round_split(input, strtol(ENTRYMAX, NULL, 10));

    /* Merge rounds and write to sorted output*/
    round_merge(output, roundNum);

    return 0;
}
