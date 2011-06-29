#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef EXSORT_H
#define EXSORT_H
#include "exsort.h"
#endif

/* Maximum length of an entry 
 * [100] Url [1] Space [9] Page views [1] Newline */
#define ENTRYLEN 111
/* Maximum filename for each round */
#define MAXFILENAME 20

/* Swap the contents of two entries */
void entry_swap(entry_t *a, entry_t *b){
    entry_t tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Split the input file into multiple sorted files */
int round_split(FILE *input, int entryMax){
    int readNum, i;
    round_t round;

    round.num = 0;
    round.entry = (entry_t*) malloc (entryMax * sizeof(entry_t));
    for (i = 0; i < entryMax; i++)
        round.entry[i].url = (char*) malloc (ENTRYLEN * sizeof(char));

    while (!feof(input)){
        for (round.entryNum = 0; round.entryNum < entryMax; round.entryNum++){
            readNum = fscanf(input, "%s %i\n", round.entry[round.entryNum].url, &round.entry[round.entryNum].views);
            if (readNum != 2)
                break;
        }

        /* Quick sort all the entries */
        sort_quick(round.entry, 0, round.entryNum - 1);

        round_write_file(&round, round.num);
        round.num++;
    }

    for (i = 0; i < entryMax; i++)
        free(round.entry[i].url);
    free(round.entry);
    fclose(input);
    return round.num;
}

/* Take the sorted round, and write it to a file */
void round_write_file(round_t *round, int roundCur){
    FILE *roundFile;
    char roundName[MAXFILENAME];
    int i;

    /* Create file to store entries for each round */
    /* Round filename is rodada-(Current Round Number).txt */
    sprintf(roundName, "rodada-%i.txt", roundCur);
    roundFile = fopen(roundName, "w");
    for (i = 0; i < round->entryNum; i++){
        fprintf(roundFile, "%s %i\n", round->entry[i].url, round->entry[i].views);
    }
    fclose(roundFile);
}

/* Reopen all the rounds generated and merge them into a single sorted output */
void round_merge(FILE *output, int roundNum){
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

        if (!feof(roundFile[entryFirst.round])){ 
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

/* Recursive quicksort uses shell sort if array below size defined by SHELLSTART */
void sort_quick(entry_t* entry, int start, int end){
    int pivot;

    if (start < end){
        pivot = end;
        pivot = sort_quick_partition(entry, pivot, start, end);
        sort_quick(entry, start, pivot - 1);
        sort_quick(entry, pivot + 1, end);
    }
}

/* Auxiliary function for sort_quick to divide and conquer quicksort */
int sort_quick_partition(entry_t* entry, int pivot, int start, int end){
    int i, j;

    entry_swap(&entry[pivot], &entry[end]);
    j = start;
    for (i = start; i < end; i++)
        if (entry[i].views > entry[pivot].views || (
                    entry[i].views == entry[pivot].views && 
                    strcmp(entry[i].url, entry[pivot].url) == -1
                    )
           ){
            entry_swap(&entry[i], &entry[j]);
            j++;
        }
    entry_swap(&entry[j], &entry[end]);
    return j;
}

/* Function to maintain heap property */
void queue_fix(entry_t *heap, int heapSize, int father){
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
        entry_swap(&heap[largest], &heap[father]); 
        queue_fix(heap, heapSize, largest);
    }
}

/* Build a priority queue as a heap structure */
void queue_build(entry_t *heap, int heapSize){
    int i;

    for (i = ((double) heapSize - 1) / 2; i >= 0; i--)
        queue_fix(heap, heapSize, i);
}

/* Return and remove the first element of the queue */
entry_t queue_pop(entry_t *heap, int *heapSize){
    entry_t largest;

    largest = heap[0];
    heap[0] = heap[(*heapSize)-- - 1];
    queue_fix(heap, *heapSize, 0);
    return largest;
}

/* Insert an element into the queue and reorder */
void queue_push(entry_t *heap, int *heapSize, entry_t insert){
    int i;

    heap[*heapSize] = insert;
    i = *heapSize;
    (*heapSize)++;
    while (i > 0 && (
                heap[(i - 1) / 2].views < heap[i].views || (
                    heap[(i - 1) / 2].views == heap[i].views && 
                    strcmp(heap[(i - 1) / 2].url, heap[i].url) == 1
                    )
                )
          ){
        entry_swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}
