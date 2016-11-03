CC=gcc
CFLAGS= -Wall -std=c99 -D_POSIX_C_SOURCE=200112L
OBJ=csc60mshell.o
EXE=csc60mshell

all: $(EXE)
$(EXE): $(OBJ)
		$(CC) $(OBJ) -o $(EXE)
csc60mshell.o: csc60mshell.c
		$(CC) $(CFLAGS) -c csc60mshell.c
clean: 
		rm $(OBJ)
