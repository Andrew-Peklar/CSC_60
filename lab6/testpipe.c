#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_LENGTH 100
int main(int argc, char ** argv) {
   int fildes[2]; 
   char result[] = "";
   pipe(fildes);
   int pid = fork();
   if (pid > 0)
      write(fildes[1], "I am writing into the pipe", MAX_LENGTH);
   else {
      read(fildes[0], result, MAX_LENGTH);
      printf("I read <<%s>> from the pipe.\n", result);
      _exit(0);
    }
   exit(0);
}

