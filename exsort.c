#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef EXSORT_H
#define EXSORT_H
#include "exsort.h"
#endif

/* Maximum length of an entry 
 * [100] Url [1] Space [9] Page views [1] Newline */
#define ENTRYLEN 111
/* Threshold to use shell sort instead of quick */
#define SHELLSTART 1
/* Maximum filename for each round */
#define MAXFILENAME 20


/* Discover file's filesize */
unsigned long file_len(FILE* file){
    unsigned long start;
    unsigned long len;

    start = ftell(file);
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, start, SEEK_SET);
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
void swap_entry(entry_t *a, entry_t *b){
    entry_t tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Shell sort */
void sort_shell(entry_t* entryArr, int start, int end){
    int i, j, h;
    entry_t tmp;

    for (h = 1; h < end - start; h = h * 3 + 1);
    while (h > 0){
        h = (h - 1) / 3;
        for (i = start + h; i <= end; i++){
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

/* Auxiliary function for sort_quick to divide and conquer quicksort */
int sort_quick_partition(entry_t* entryArr, int pivot, int start, int end){
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

/* Recursive quicksort uses shell sort if array below size defined by SHELLSTART*/
void sort_quick(entry_t* entryArr, int start, int end){
    int pivot;

    if (start < end){
        if (end - start < SHELLSTART)
            sort_shell(entryArr, start, end);
        else {
            pivot = end;
            pivot = sort_quick_partition(entryArr, pivot, start, end);
            sort_quick(entryArr, start, pivot - 1);
            sort_quick(entryArr, pivot + 1, end);
        }
    }
}

/* Take the string of a round, sort, and write it to a file */
void round_qsort(round_t *roundUnsrt, int roundCur){
    entry_t *entryParsed;
    FILE *roundFile;
    char roundName[MAXFILENAME];
    int entryNum, i;

    /* Parsing the round string */
    entryNum = 1 + str_numchar(roundUnsrt->str, '\n');
    entryParsed = (entry_t*) malloc (entryNum * sizeof(entry_t));
    entryParsed[0].url = strtok(roundUnsrt->str, " \n"); 
    entryParsed[0].views = (int) strtol(strtok(NULL, " \n"), NULL, 10);
    for (i = 1; i < entryNum; i++){
        entryParsed[i].url = strtok(NULL, " \n"); 
        entryParsed[i].views = (int) strtol(strtok(NULL, " \n"), NULL, 10);
    }

    /* Quick sort all the entries */
    sort_quick(entryParsed, 0, entryNum - 1);

    /* Create file to store entries for each round */
    /* Round filename is rodada-(Current Round Number).txt */
    sprintf(roundName, "rodada-%i.txt", roundCur);
    roundFile = fopen(roundName, "w");
    for (i = 0; i < entryNum; i++)
        fprintf(roundFile, "%s %i\n", entryParsed[i].url, entryParsed[i].views);

    free(entryParsed);
    fclose(roundFile);
}

/* Split the input file into multiple sorted files. */
int round_split(FILE *input, int entryMax){
    round_t *roundUnsrt;
    int i, roundNum, entryEnd;
    unsigned long inputLen;
    unsigned long end;

    inputLen = file_len(input);
    roundNum = (double) inputLen / (ENTRYLEN * entryMax) + 1;
    roundUnsrt = (round_t*) malloc (roundNum * sizeof(round_t));

    for (i = 0; i < roundNum; i++){
        roundUnsrt[i].str = (char*) malloc ((entryMax * ENTRYLEN + 1) * sizeof(char));
        roundUnsrt[i].len = ENTRYLEN * entryMax;
        fread(roundUnsrt[i].str, roundUnsrt[i].len, sizeof(char), input);
        end = inputLen >= (unsigned) roundUnsrt[i].len ? (unsigned) roundUnsrt[i].len : inputLen;
        roundUnsrt[i].str[end + 1] = '\0';
        entryEnd = strrchr(roundUnsrt[i].str, '\n') - roundUnsrt[i].str;

        fseek(input, (entryEnd + 1) - roundUnsrt[i].len, SEEK_CUR);
        roundUnsrt[i].len = entryEnd;
        roundUnsrt[i].str[roundUnsrt[i].len] = '\0';
        inputLen -= roundUnsrt[i].len;

        round_qsort(&roundUnsrt[i], i);
        free(roundUnsrt[i].str);
    }
    free(roundUnsrt);
    return roundNum;
}

/* Function to maintain heap property */
void queue_heapify(entry_t *heap, int heapSize, int father){
    int left = father * 2 + 1;
    int right = father * 2 + 2;
    int largest;

    if (left < heapSize && ( heap[left].views > heap[father].views || (
                    heap[left].views == heap[father].views && 
                    strcmp(heap[left].url, heap[father].url) == -1
                    )
                )
       )
        largest = left;
    else
        largest = father;
    if (right < heapSize && ( heap[right].views > heap[largest].views || ( 
                    heap[right].views == heap[largest].views && 
                    strcmp(heap[right].url, heap[largest].url) == -1
                    )
                )
       )
        largest = right;
    if (largest != father){
        swap_entry(&heap[largest], &heap[father]); 
        queue_heapify(heap, heapSize, largest);
    }
}

/* Build a priority queue as a heap structure */
void queue_build(entry_t *heap, int heapSize){
    int i;

    for (i = ((double) heapSize - 1) / 2; i >= 0; i--)
        queue_heapify(heap, heapSize, i);
}

/* Return and remove the first element of the queue */
entry_t queue_pop(entry_t *heap, int *heapSize){
    entry_t largest;

    largest = heap[0];
    heap[0] = heap[(*heapSize)-- - 1];
    queue_heapify(heap, *heapSize, 0);
    return largest;
}

/* Insert an element into the queue and reorder */
void queue_push(entry_t *heap, int *heapSize, entry_t insert){
    int i;

    heap[*heapSize] = insert;
    i = *heapSize;
    (*heapSize)++;
    while (i > 0 && (
                heap[i / 2].views < heap[i].views || (
                    heap[i / 2].views == heap[i].views && 
                    strcmp (heap[i / 2].url, heap[i].url) == 1
                    )
                )
          ){
        swap_entry(&heap[i], &heap[i / 2]);
        i = i / 2;
    }
}

/* Reopen all the rounds generated and intersperse them into a single sorted output */
void round_intersperse(FILE *output, int roundNum){
    int i, queueSize;
    char roundName[MAXFILENAME];
    FILE **roundFile;
    entry_t *entryQueue, entryFirst, entryInsert;

    roundFile = (FILE**) malloc (roundNum * sizeof(FILE*));
    entryQueue = (entry_t*) malloc (roundNum * sizeof(entry_t));
    queueSize = roundNum;

    /* Reopen each round and add first element to queue */
    for (i = 0; i < roundNum; i++){
        sprintf(roundName, "rodada-%i.txt", i);
        roundFile[i] = fopen (roundName, "r");
        entryQueue[i].round = i;
        entryQueue[i].url = (char*) malloc (ENTRYLEN * sizeof(char));
        fscanf(roundFile[i], "%s %i\n", entryQueue[i].url, &entryQueue[i].views);
    }
    queue_build(entryQueue, queueSize);

    /* Until queue is empty write the highest priority entry and insert into queue next element from
     * the highest entries original round. */
    while (queueSize > 0){
        entryFirst = queue_pop(entryQueue, &queueSize);
        fprintf(output, "%s %i\n", entryFirst.url, entryFirst.views);
        free(entryFirst.url);

        if ((unsigned) ftell(roundFile[entryFirst.round]) != file_len(roundFile[entryFirst.round])){ 
            entryInsert.round = entryFirst.round;
            entryInsert.url = (char*) malloc (ENTRYLEN * sizeof(char));
            fscanf(roundFile[entryInsert.round], "%s %i\n", entryInsert.url, &entryInsert.views);
            queue_push(entryQueue, &queueSize, entryInsert);
        }
    }

    /* Clean up all the temporary rounds generated */
    for (i = 0; i < roundNum; i++){
        fclose(roundFile[i]);
        sprintf(roundName, "rodada-%i.txt", i);
        remove(roundName);
    }
    free(roundFile);
    free(entryQueue);
}
