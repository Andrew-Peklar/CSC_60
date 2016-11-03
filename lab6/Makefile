# Simple Makefile for mystat program
# pipe.c error_functions.c get_num.c curr_time.c semun.h

CC=gcc
CFLAGS=-g -Wall
OBJ=pipe.o get_num.o curr_time.o error_functions.o
EXE=pipe

all: stuff

stuff: pipe.c get_num.c curr_time.c error_functions.c
	gcc -o pipe pipe.c get_num.c curr_time.c error_functions.c