#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

typedef struct round{
    char* str;
    int len;
} round;

typedef struct entry{
    char *url;
    int *views;
} entry;

/* Discover file's filesize */
unsigned long file_len(FILE* file){
    unsigned long start;
    unsigned long len;
    start = ftell(file);
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, start);
    return len;
}

void round_qsort(round* roundUnsrt){
    /*
    find number of entities
    allocate array to store pointers to entry
    break them up
    allocate and make each pointer point to an entry
    sort the pointers to entries
    */
}

/* Split the input file into multiple files. */
int file_rounds_split(FILE *input, int entriesMax, int entryLen) {
    round* roundUnsrt;
    int i, roundNum, entryEnd;
    unsigned long inputLen;

    inputLen = file_len(input);
    roundNum = ceil((double) inputLen / (entryLen * entriesMax));
    roundUnsrt = (round*) malloc (roundNum * sizeof(round));

    for (i = 0; i < roundNum; i++){
        roundUnsrt[i].str = (char*) malloc ((entriesMax * entryLen + 1) * sizeof(char));
        roundUnsrt[i].len = entryLen * entriesMax;
        fread(roundUnsrt[i].str, roundUnsrt[i].len, sizeof(char), input);

        /* Fix the round when it cuts off an entry to make it go in the next round. */
        for (entryEnd = roundUnsrt[i].len; roundUnsrt[i].str[entryEnd] != '\n'; entryEnd--);
        assert(entryEnd <= roundUnsrt[i].len);
        fseek(input, (entryEnd + 1) - roundUnsrt[i].len, SEEK_CUR);
        roundUnsrt[i].len = entryEnd;
        roundUnsrt[i].str[roundUnsrt[i].len] = '\0';

        /*sort round*/
        round_qsort(&roundUnsrt[i]);

        /*open round */
        /*write round */
        /*close round */
        printf("%i %i\n%s", roundUnsrt[i].len, strlen(roundUnsrt[i].str), roundUnsrt[i].str);
        printf("\n###\n");
        /* FREE ROUND MEMORY LATER */
    }
    return roundNum;
}

/* Discover the number of times a character appears in a string */
unsigned long str_numchar(char *str, char key){
    unsigned long occurrences = 0, i;
    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == key)
            occurrences++;
    return occurrences;
}
