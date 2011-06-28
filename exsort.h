#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

/* Discover file's filesize */
unsigned long file_len(FILE* file);

/* Discover the number of times a character appears in a string */
unsigned long str_numchar(char *str, char key);

/* Swap the contents of two entries */
void entry_swap(entry_t *a, entry_t *b);

/* Shell sort */
void sort_shell(entry_t* entryArr, int start, int end);

/* Auxiliary function for sort_quick to divide and conquer quicksort */
int sort_quick_partition(entry_t* entryArr, int pivot, int start, int end);

/* Recursive quicksort uses shell sort if array below size defined by SHELLSTART*/
void sort_quick(entry_t* entryArr, int start, int end);

/* Take the round and write it to a file */
void round_write_file(round_t *roundUnsrt, int roundCur);

/* Split the input file into multiple sorted files. */
int round_split(FILE *input, int entryMax);

/* Function to maintain heap property */
void queue_heapify(entry_t *heap, int heapSize, int father);

/* Build a priority queue as a heap structure */
void queue_build(entry_t *heap, int heapSize);

/* Return and remove the first element of the queue */
entry_t queue_pop(entry_t *heap, int *heapSize);

/* Insert an element into the queue and reorder */
void queue_push(entry_t *heap, int *heapSize, entry_t insert);

/* Reopen all the rounds generated and merge them into a single sorted output */
void round_merge(FILE *output, int roundNum);
