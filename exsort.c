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
void swap_entries(entry *a, entry *b){
    entry tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

int sort_quick_partition(entry* entries, int pivot, int start, int end){
    int i, j;
    swap_entries(&entries[pivot], &entries[end]);
    j = start;
    for (i = start; i < end; i++)
        if (entries[i].views > entries[pivot].views || (
                    entries[i].views == entries[pivot].views && 
                    strcmp(entries[i].url, entries[pivot].url) == -1
                    )
           ){
            swap_entries(&entries[i], &entries[j]);
            j++;
        }
    swap_entries(&entries[j], &entries[end]);
    return j;
}

void sort_quick_aux(entry* entries, int start, int end){
    int pivot;
    if (start < end){
        /* Improve pivot */
        pivot = end;
        pivot = sort_quick_partition(entries, pivot, start, end);
        sort_quick_aux(entries, start, pivot - 1);
        sort_quick_aux(entries, pivot + 1, end);
    }
}

void sort_quick(entry* entries, int entriesNum){
    sort_quick_aux(entries, 0, entriesNum - 1);
}

void sort_shell(entry* entries, int entriesNum){
    int i, j, h;
    entry tmp;
    for (h = 1; h < entriesNum; h = h * 3 + 1);
    while (h > 0){
        h = (h - 1) / 3;
        for (i = h; i < entriesNum; i++){
            tmp = entries[i];
            j = i;
            while (j >= h && (
                        entries[j - h].views < tmp.views || (
                            entries[j - h].views == tmp.views &&
                            strcmp(entries[j - h].url, tmp.url) == 1
                            )
                        )
                  ){
                entries[j] = entries[j - h];
                j -= h;
            }
            entries[j] = tmp;
        }
    }
}


void round_qsort(round *roundUnsrt, int roundCur){
    entry *entriesDec;
    FILE *roundSrt;
    char roundName[10];
    int entriesNum, i;

    /* Parsing the round string */
    entriesNum = 1 + str_numchar(roundUnsrt->str, '\n');
    entriesDec = (entry*) malloc (entriesNum * sizeof(entry));
    entriesDec[0].url = strtok(roundUnsrt->str, " \n"); 
    entriesDec[0].views = (int) strtol(strtok(NULL, " \n"), NULL, 10);
    for (i = 1; i < entriesNum; i++){
        entriesDec[i].url = strtok(NULL, " \n"); 
        entriesDec[i].views = (int) strtol(strtok(NULL, " \n"), NULL, 10);
    }

    sort_quick(entriesDec, entriesNum);
    /* Round filename is Rod_(Current Round Number) */
    sprintf(roundName, "Rod_%i", roundCur);
    roundSrt = fopen(roundName, "w");
    for (i = 0; i < entriesNum; i++)
        fprintf(roundSrt, "%s %i\n", entriesDec[i].url, entriesDec[i].views);
    fclose(roundSrt);
}

/* Split the input file into multiple files. */
int file_rounds_split(FILE *input, int entriesMax, int entryLen) {
    round *roundUnsrt;
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

        printf("\n###\n");
        printf("%i %i\n%s", roundUnsrt[i].len, strlen(roundUnsrt[i].str), roundUnsrt[i].str);


        /*sort round*/
        round_qsort(&roundUnsrt[i], i);

        /*
        for (i = 0; i < 9; i++)
            printf("%s::%i\n", entriesDec[i].url, entriesDec[i].views);
            */

        /*open round */
        /*write round */
        /*close round */
        /* FREE ROUND MEMORY LATER */
    }
    return roundNum;
}
