#!/bin/bash
if [ $# -eq 0 ] 
   then
    echo "No filename supplied"
    echo "try i.e. gcc_stat.sh t_stat.c"
    exit
fi
gcc -o csc60_stat error_functions.c file_perms.c $1
