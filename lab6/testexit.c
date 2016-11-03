#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main () {
   int status;
   printf("hello");
   pid_t pid = fork();
   if (pid == 0) {
     sleep (2);
     _exit(0);
   } else {
     wait(&status);
     printf(", bye\n");
   }
   exit(0);
}
