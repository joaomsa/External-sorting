#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Stores contents and info about each url entry from input*/
typedef struct entry_t{
    char *url;
    int views;
    int round;
} entry_t;

/* Stores contents and info about each round split off from input */
typedef struct round_t{
    entry_t *entry;
    int num, entryNum;
} round_t;

/* Swap the contents of two entries */
void entry_swap(entry_t *a, entry_t *b);

/* Split the input file into multiple sorted files. */
int round_split(FILE *input, int entryMax);

/* Take the round and write it to a file */
void round_write_file(round_t *round, int roundCur);

/* Reopen all the rounds generated and merge them into a single sorted output */
void round_merge(FILE *output, int roundNum);

/* Recursive quicksort uses shell sort if array below size defined by SHELLSTART*/
void sort_quick(entry_t* entry, int start, int end);

/* Auxiliary function for sort_quick to divide and conquer quicksort */
int sort_quick_partition(entry_t* entry, int pivot, int start, int end);

/* Function to maintain heap property */
void queue_fix(entry_t *heap, int heapSize, int father);

/* Build a priority queue as a heap structure */
void queue_build(entry_t *heap, int heapSize);

/* Return and remove the first element of the queue */
entry_t queue_pop(entry_t *heap, int *heapSize);

/* Insert an element into the queue and reorder */
void queue_push(entry_t *heap, int *heapSize, entry_t insert);

