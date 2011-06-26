CC=gcc
CFLAGS=-Wall -Wextra -ansi -pedantic -g -O0
SOURCES=main.c exsort.c
EXECUTABLE=exsort.exe
all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)
