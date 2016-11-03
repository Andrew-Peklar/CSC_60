#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
int main(void)
{
   pid_t pid;
   int status, i;
   int choice = 1;
   int waitpid_status;
   printf("1: WNOHANG, 2: WUNTRACED: ");
   scanf("%d",&choice); 
   for (i = 0;i < 3; i++) {
      if((pid = fork()) == 0 ) 
      { /* child */
      	printf("I am a child with pid = %d\n",
      	getpid());
      	sleep(2);
      	printf("child terminates\n");
      	return(0);
      }
      else 
      {
        continue;
      } /* parent */
   }
   i = 0;
   while (1)
           {
                if (choice == 1)
                  waitpid_status = waitpid(0, &status, WNOHANG);
                else
                  waitpid_status = waitpid(0, &status, WUNTRACED);
                printf("Finished waitpid for child processes\n");
                if( WIFSTOPPED(status) )
                {
                        printf("child stopped, signal(%d)\n",
                        WSTOPSIG(status));
                        continue;
                }
                else if( WIFEXITED(status) ) {
                       i += 1;
                       printf("normal termination with status (%d)\n",
                       WEXITSTATUS(status));
                       if (i == 3)  {
                          printf("All children terminated\n");
                          return(0);
                       }
                       else
                          continue;
                }
                else if (WIFSIGNALED(status) && waitpid_status != 0) {
                       printf("%d value \n", waitpid_status);
                       printf("abnormal termination, signal(%d)\n",
                       WTERMSIG(status));
                       return(-1);
                       }
                else if (waitpid_status == 0) {
                //       printf("Waiting for child to wake up ...\n");
                       continue;
                       }
            } /* while */
} /* main */

