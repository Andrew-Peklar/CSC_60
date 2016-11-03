# Simple Makefile for csc60_stat program
CC=gcc
CFLAGS=-g -Wall
OBJ=file_perms.o error_functions.o csc60_stat.o
EXE=csc60_stat

all: $(EXE)
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE)
csc60_stat.o: file_perms.h tlpi_hdr.h csc60_stat.c
	$(CC) $(CFLAGS) -c csc60_stat.c
file_perms.o: file_perms.h file_perms.c
	$(CC) $(CFLAGS) -c file_perms.c
error_functions.o: error_functions.h tlpi_hdr.h ename.c.inc error_functions.c
	$(CC) $(CFLAGS) -c error_functions.c
clean:
	rm $(OBJ)
