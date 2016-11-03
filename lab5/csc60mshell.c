/* Author:  Andrew Peklar
 * Title:   Assingment 5 - csc60mshell
 * Teacher: Prof. Nguyen
 * Date:    11/25/2015 
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAXPATH 256
#define MAXLINE 80
#define MAXARGS 20
#define MAXJOBS 20

void process_input(int argc, char **argv);
int parseline(char *cmdline, char **argv);
void _exit_error(char* s, bool __exit);

// Declare job array struct
struct job_array {
    char command[80];
    int process_id;
    int job_number;
};

/*Array declaration */
struct job_array myJobs[MAXJOBS]; 
/* Number of entries in array */
int processes = 0;

/*Returns flag to indicate jobs are/are not running*/
int jobsActive()
{
  int i, flag = 0;
  for (i = 0; i < processes; i++)
    if (myJobs[i].process_id != 0) {
       flag = 1;
       break;
    }
  return (flag);
}

/* Loops through jobs then marks finished ones with new pid = 0 */
void childHandler(int sig){
  int i,status;
  pid_t pid;
  while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
    for(i = 0; i < processes; i++){
       if(myJobs[i].process_id == pid){
            if (WIFEXITED(status)) 
                printf("\n[%i]   Finished.   %s", myJobs[i].job_number, myJobs[i].command); 
            else if (WIFSIGNALED(status)) 
                printf("\n[%i]   Killed.     %s", myJobs[i].job_number, myJobs[i].command); 
            else if (WIFSTOPPED(status)) 
                printf("\n[%i]   Stopped.    %s", myJobs[i].job_number, myJobs[i].command); 
            /* Mark for deletion pid = 0 */
            myJobs[i].process_id = 0; 
       }
    }
  }
}

int main(void) {
    pid_t pid;
    int argc=0;
    int status;
    int background = 0;
    char cmdline[MAXLINE];
    char *argv[MAXARGS];
    char path[MAXPATH];

    printf("Welcome to csc60mshell!\n");

    /* Initialize jobs array */
    int i;
    for (i=0;i < MAXJOBS;i++)
        myJobs[i].process_id = 0;
    
    /* Cntrl-C cmdline (ignore SIGINT signal) */
    struct sigaction handler;
    handler.sa_handler = SIG_IGN;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIGINT, &handler, NULL);

    
    /* SIGCHLD ==> signal handler*/
    struct sigaction childaction;
    childaction.sa_handler = childHandler;
    sigemptyset(&childaction.sa_mask);
    childaction.sa_flags = 0;
    sigaction(SIGCHLD, &childaction, NULL);

    for (;;) {
        printf("[csc60mshell]> ");
        while (fgets(cmdline, MAXLINE, stdin) == NULL)  continue; 
        
        /*Copy command text for printing use */
        char tcmdline[80];
        strcpy(tcmdline,cmdline);

        argc = parseline(cmdline, argv);

        /* This is where the background/foreground assingment happens */
        /* Checks input for '&' to mean background process */
        if(argc==0)     continue;                       //no argument ==> continue
        if(argc > 1 && strcmp(argv[argc-1],"&")==0){    //checks for background 
            if (processes == MAXJOBS) {                 //checking to see if more jobs can fit
                printf("Max jobs number reached\n"); 
                printf("Run job in front ground\n");    
                background = 0;                         //Front gound
            }
            else 
                background = 1;                         //Run job in background 
                argv[argc-1] = NULL;
                argc--; 
        } else
            background = 0;

        /* Exit Command; checks if jobs are active before letting user exit */
        /* Will prevent user from exiting if jobs are active*/  
        if(strcmp(argv[0],"exit")==0) {
            if (jobsActive()) {
                printf("\nERROR: You have %i active processes.",processes);
                printf("\nPlease terminate them before exiting\n");
            }
            else
                exit(EXIT_SUCCESS);
        }

        /* Jobs command */
        else if(strcmp(argv[0],"jobs")==0){
            if (jobsActive()) {                         //Checking if there are active jobs to print
                printf("Current Jobs:\n");
                printf("Num    Status     Entry \n");  
                int i;
                for(i = 0; i < MAXJOBS; i++){
                    if (myJobs[i].process_id != 0)
                        printf("[%d]    Running \t  %s \n", myJobs[i].job_number, myJobs[i].command);  
                }
            }
        }

        /* Null input loop */
        if (*argv == NULL) continue;

        /* cmdline print working directory */
        if (strcmp("pwd", argv[0]) == 0) {
            printf("%s\n", getenv("PWD"));
            continue;
        }

        /* error handling for invalid cmdlines */
        if (*argv[0] == '<' || *argv[0] == '>') {
            _exit_error("No cmdline.", false);
            continue;
        }

        /* cmdline cd */
        if (strcmp("cd", argv[0]) == 0) {
            // Not home
            if (argc > 1) chdir(argv[1]);
            // Home
            else chdir(getenv("HOME"));

            /* Updates pwd */
            getcwd(path, MAXPATH);
            setenv("PWD", path, 1);
            continue;
        }

        else {
            pid = fork();
            if(pid == -1) perror("Shell programming error, unable to fork.\n");

            /* Here is the child process */
            else if( pid == 0) {   
                /* Sigaction handler for child process */
                /* Default setting of SIGINT ==> enables termination */
                struct sigaction interrupt;
                interrupt.sa_handler = SIG_DFL;
                sigemptyset(&interrupt.sa_mask);
                interrupt.sa_flags = 0;
                sigaction(SIGINT, &interrupt, NULL);

                /* If background ==> give new process group */
                /* Prevents user from accidently killing it */
                if(background)  setpgid(0,0);           
                process_input(argc, argv);
            }
            /* Here the parent process begins */
            else if(background) {

                /* Logs jobs infor to array and removes */
                tcmdline[strlen(tcmdline)-2] = '\0'; 
                strcpy(myJobs[processes].command,tcmdline);  

                myJobs[processes].job_number = processes;
                myJobs[processes].process_id = pid;
                processes++;   
            }else{
                if (wait(&status) == -1 )   perror("Shell Program error");
                else                        printf("Child returned status: %d\n",status);

            }
        }
    }
    return 0;
}

void process_input(int argc, char **argv) {
    int fd;
    bool flag = false;

    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '<') {
            // error checking
            if (flag) _exit_error("Can't have two input redirects.", true);
            if (argv[i + 1] == NULL) _exit_error("No input redictions file specified.", true);
            // open a fd
            if((fd = open(argv[i + 1], O_RDONLY)) < 0) perror("open");
            // dup the fd and close the fd
            if(dup2(fd, 0) < 0) perror("dup2");
            close(fd);
            argv[i] = NULL;
            flag = true;
        } else if (strcmp(">", argv[i]) == 0) {
            if (argv[i + 1] == NULL) _exit_error("No output file specified.", true);
            if ((fd = open(argv[i + 1], O_TRUNC | O_WRONLY | O_CREAT, 0666)) < 0) perror("open");
            if (dup2(fd, 1) < 0) perror("dup2");
            close(fd);
            argv[i] = NULL;
        } else if (strcmp(">>", argv[i]) == 0) {
            if (argv[i + 1] == NULL) _exit_error("No output file specified.", true);
            if ((fd = open(argv[i + 1], O_APPEND | O_WRONLY | O_CREAT, 0666)) < 0) perror("open");
            if (dup2(fd, 1) < 0) perror("dup2");
            close(fd);
            argv[i] = NULL;
        }
    }

    if (execvp(*argv, argv) == -1) {
        //perror("process_input, shell program.");
        _exit(0);
    }
}

/*needed to reference _exit_error */
void _exit_error(char* s, bool __exit) {
    printf("ERROR: %s\n", s);
    if (__exit) _exit(1);
}


int parseline(char *cmdline, char **argv) {
    int count = 0;
    char *separator = " \n\t";
    argv[count] = strtok(cmdline, separator);

    while ((argv[count] != NULL) && (count + 1 < MAXARGS)) {
        argv[++count] = strtok((char *) 0, separator);
    }

    return count;
}
