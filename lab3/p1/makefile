#Makefile for csc60_stat_make_file program


CC=gcc
CFLAGS=-g -Wall
OBJ=file_perms.o error_functions.o csc60_stat_make_file.o
EXE=csc60_stat_make_file

all: $(EXE)
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE)
csc60_stat_make_file.o: file_perms.h tlpi_hdr.h csc60_stat_make_file.c
	$(CC) $(CFLAGS) -c csc60_stat_make_file.c
file_perms.o: file_perms.h file_perms.c
	$(CC) $(CFLAGS) -c file_perms.c
error_functions.o: error_functions.h tlpi_hdr.h ename.c.inc error_functions.c
	$(CC) $(CFLAGS) -c error_functions.c
clean:
	rm $(OBJ)
