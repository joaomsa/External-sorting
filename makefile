CC=gcc
CFLAGS=-Wall -Wextra -ansi -g -O0 -lm
SOURCES=main.c
EXECUTABLE=exsort.exe
all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)
