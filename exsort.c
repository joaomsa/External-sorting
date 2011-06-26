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
    int views;
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

/* Discover the number of times a character appears in a string */
unsigned long str_numchar(char *str, char key){
    unsigned long occurrences = 0, i;
    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == key)
            occurrences++;
    return occurrences;
}

/* Swap the contents of two entries */
void swap_entry(entry *a, entry *b){
    entry tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Auxliary function for sort_quick to divide and conquer quicksort*/
int sort_quick_partition(entry* entryArr, int pivot, int start, int end){
    int i, j;
    swap_entry(&entryArr[pivot], &entryArr[end]);
    j = start;
    for (i = start; i < end; i++)
        if (entryArr[i].views > entryArr[pivot].views || (
                    entryArr[i].views == entryArr[pivot].views && 
                    strcmp(entryArr[i].url, entryArr[pivot].url) == -1
                    )
           ){
            swap_entry(&entryArr[i], &entryArr[j]);
            j++;
        }
    swap_entry(&entryArr[j], &entryArr[end]);
    return j;
}

/* Recursive quicksort */
void sort_quick(entry* entryArr, int start, int end){
    int pivot;
    if (start < end){
        /* Improve pivot */
        pivot = end;
        pivot = sort_quick_partition(entryArr, pivot, start, end);
        sort_quick(entryArr, start, pivot - 1);
        sort_quick(entryArr, pivot + 1, end);
    }
}

/* Shell sort */
void sort_shell(entry* entryArr, int entryNum){
    int i, j, h;
    entry tmp;
    for (h = 1; h < entryNum; h = h * 3 + 1);
    while (h > 0){
        h = (h - 1) / 3;
        for (i = h; i < entryNum; i++){
            tmp = entryArr[i];
            j = i;
            while (j >= h && (
                        entryArr[j - h].views < tmp.views || (
                            entryArr[j - h].views == tmp.views &&
                            strcmp(entryArr[j - h].url, tmp.url) == 1
                            )
                        )
                  ){
                entryArr[j] = entryArr[j - h];
                j -= h;
            }
            entryArr[j] = tmp;
        }
    }
}

/* Take the string of a round, sort, and write it to a file */
void round_qsort(round *roundUnsrt, int roundCur){
    entry *entryParsed;
    FILE *roundFile;
    char roundName[10];
    int entryNum, i;

    /* Parsing the round string */
    entryNum = 1 + str_numchar(roundUnsrt->str, '\n');
    entryParsed = (entry*) malloc (entryNum * sizeof(entry));
    entryParsed[0].url = strtok(roundUnsrt->str, " \n"); 
    entryParsed[0].views = (int) strtol(strtok(NULL, " \n"), NULL, 10);
    for (i = 1; i < entryNum; i++){
        entryParsed[i].url = strtok(NULL, " \n"); 
        entryParsed[i].views = (int) strtol(strtok(NULL, " \n"), NULL, 10);
    }

    sort_quick(entryParsed, 0, entryNum - 1);

    /* Create file to store entries for each round */
    /* Round filename is R_(Current Round Number) */
    sprintf(roundName, "R_%i", roundCur);
    roundFile = fopen(roundName, "w");
    for (i = 0; i < entryNum; i++)
        fprintf(roundFile, "%s %i\n", entryParsed[i].url, entryParsed[i].views);
    fclose(roundFile);
}

/* Split the input file into multiple sorted files. */
int file_rounds_split(FILE *input, int entryMax, int entryLen) {
    round *roundUnsrt;
    int i, roundNum, entryEnd;
    unsigned long inputLen;

    inputLen = file_len(input);
    roundNum = ceil((double) inputLen / (entryLen * entryMax));
    roundUnsrt = (round*) malloc (roundNum * sizeof(round));

    for (i = 0; i < roundNum; i++){
        roundUnsrt[i].str = (char*) malloc ((entryMax * entryLen + 1) * sizeof(char));
        roundUnsrt[i].len = entryLen * entryMax;
        fread(roundUnsrt[i].str, roundUnsrt[i].len, sizeof(char), input);

        /* Fix the round when it cuts off an entry to make it go in the next round. */
        for (entryEnd = roundUnsrt[i].len; roundUnsrt[i].str[entryEnd] != '\n'; entryEnd--);
        assert(entryEnd <= roundUnsrt[i].len);
        fseek(input, (entryEnd + 1) - roundUnsrt[i].len, SEEK_CUR);
        roundUnsrt[i].len = entryEnd;
        roundUnsrt[i].str[roundUnsrt[i].len] = '\0';

        round_qsort(&roundUnsrt[i], i);
        /* FREE ROUND MEMORY LATER */
    }
    return roundNum;
}

void file_rounds_intersperse(FILE *output, int roundNum, int entryLen){
    int i;
    char roundName[10];
    FILE **roundFile;
    entry *entryQueue;
    roundFile = (FILE**) malloc (roundNum * sizeof(FILE*));
    entryQueue = (entry*) malloc (roundNum * sizeof(entry));
    for (i = 0; i < roundNum; i++){
        sprintf(roundName, "R_%i", i);
        roundFile[i] = fopen (roundName, "r");
        assert(roundFile[i] != NULL);
        entryQueue[i].url = (char*) malloc (entryLen * sizeof(char));
        fscanf(roundFile[i], "%s %i\n", entryQueue[i].url, &entryQueue[i].views);
    }

    for (i = 0; i < roundNum; i++)
        printf("%s %i\n", entryQueue[i].url, entryQueue[i].views);

    fprintf(output, "REMOVE ME LATER");
}
