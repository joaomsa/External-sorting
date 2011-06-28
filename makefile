CC=gcc
#CFLAGS=-Wall -Wextra -ansi -g -O0 -lm -Q -v -da
CFLAGS=-Wall -Wextra -ansi -g -O0 -lm 
SOURCES=main.c exsort.h exsort.c
EXECUTABLE=exsort.exe
all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)
