#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define MAXPATH 200
#define MAXLINE 80
#define MAXARGS 20
// Funtion Prototypes
void process_input(int argc, char **argv);
int parseline(char *cmdline, char **argv);
void _exit_error(char* s, bool __exit);
//job-array Struct
struct job_array {
    int process_id;
    char command[80];
    int job_number;
    
};
//Global Variables
struct job_array myJobs[30];
int processes = 0;
int status;
int temp;
// ChildHandler function
void childHandler(int sig) {
    int i;
    pid_t pid;
    pid = waitpid(-1, &status, WNOHANG | WUNTRACED);    
    temp = WIFEXITED(status);
    for (i = 0; i < processes ; i++) {
       if (myJobs[i].process_id == pid) {
          myJobs[i].process_id = -999; 
       }

    }
    printf("\n[%i] Done \t %s \n",myJobs[i].job_number,myJobs[i].command);
    processes--;
}


int main(void) {
    pid_t pid;
    int argc;
    char cmdline[MAXLINE], *argv[MAXARGS], path[MAXPATH];
//Sigaction Handlers
    struct sigaction h;   
    h.sa_handler = SIG_IGN;
    sigemptyset(&h.sa_mask); 
    h.sa_flags = 0;
    sigaction(SIGINT, &h, NULL);
    
    struct sigaction sa;
	sa.sa_handler = &childHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGCHLD, &sa, NULL);
//Welcom Prompt
    printf("Welcome to csc60mshell!\n");

    if (signal(SIGCHLD, childHandler) == SIG_ERR) {
    	_exit_error("No command.", false);
    }

//Main loop for main program
    for (;;) {
//Shell Prompt
        objectsLeft:printf("csc60mshell > ");

        fflush(stdout);
        fgets(cmdline, MAXLINE, stdin);
        argc = parseline(cmdline, argv);

        if (*argv == NULL) continue;

//Job Command
        if(strcmp(argv[0],"jobs") == 0) {
            printf("");
            int j;
            for(j = 0; j< processes; j++){
               printf("[%i] Running\t %s \n",myJobs[j].job_number,myJobs[j].command);
            }
            continue;
        }


//Exit Command
        if (strcmp("exit", argv[0]) == 0) {
        	if(processes > 0 ){
        		printf("Background Process Still Up, Returning To Prompt.\n" );
				goto objectsLeft;
        	} else {
        	exit(EXIT_SUCCESS);
        }

        } 

//PWD Command
        if (strcmp("pwd", argv[0]) == 0) {
            printf("%s\n", getenv("PWD"));
            continue;
        }

//Error Handler
        if (*argv[0] == '<' || *argv[0] == '>') {
            _exit_error("No command.", false);
            continue;
        }

//CD Command
        if (strcmp("cd", argv[0]) == 0) {
            // not Kansas
            if (argc > 1) chdir(argv[1]);
            // Kansas, there is no place like home Toto.
            else chdir(getenv("HOME"));

//PWD Update
            getcwd(path, MAXPATH);
            setenv("PWD", path, 1);
            continue;
        }
//Background Process Identifier
        bool bg = false;
//Background Process Command
        if(strcmp(argv[argc-1],"&") == 0) {
            bg = true;
            argv[argc-1] = '\0';
            argc--;
            myJobs[processes].process_id = pid;
            strncpy(myJobs[processes].command, cmdline, 80); //
            myJobs[processes].job_number = processes;
            processes++;      
        }
        pid = fork();
//Fork Error Handler
        if (pid == -1) perror("Shell programming error, unable to fork.\n");
//Child Process 
        else if (pid == 0){
	if(bg) {
	     setpgid(0,0);
         }
         else{

            struct sigaction h2;
            h2.sa_handler = SIG_DFL;
            sigemptyset(&h2.sa_mask);
            h2.sa_flags = 0;
            sigaction(SIGINT, &h2, NULL);
          }

            process_input(argc, argv);
            _exit(EXIT_SUCCESS);
        } 
//Parent Process
        else {
            if(bg) {
                
		setpgid(0,0);
                sigaction(SIGCHLD, &sa, NULL);
            }else {
                if (wait(&status) == -1) perror("Shell program error");
            }
        }
    }
    return 0;
}
//Function that handles user input error
void process_input(int argc, char **argv) {
    int fd;
    bool flag = false;

    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '<') {
            
            if (flag) _exit_error("Two input redirects not allowd", true);
            if (argv[i + 1] == NULL) _exit_error("Input redictions file not specified.", true);
            
            if((fd = open(argv[i + 1], O_RDONLY)) < 0) perror("Open");
            
            if(dup2(fd, 0) < 0) perror("dup2");
            close(fd);
            argv[i] = NULL;
            flag = true;
        } else if (strcmp(">", argv[i]) == 0) {
            if (argv[i + 1] == NULL) _exit_error("Output file not specified.", true);
            if ((fd = open(argv[i + 1], O_TRUNC | O_WRONLY | O_CREAT, 0666)) < 0) perror("open");
            if (dup2(fd, 1) < 0) perror("dup2");
            close(fd);
            argv[i] = NULL;
        } else if (strcmp(">>", argv[i]) == 0) {
            if (argv[i + 1] == NULL) _exit_error("Output file not specified.", true);
            if ((fd = open(argv[i + 1], O_APPEND | O_WRONLY | O_CREAT, 0666)) < 0) perror("open");
            if (dup2(fd, 1) < 0) perror("dup2");
            close(fd);
            argv[i] = NULL;
        }
    }

    if (execvp(*argv, argv) == -1) {
        perror("process_input, shell program.");
        _exit(-1);
    }
}

//Exit error Handler
void _exit_error(char* s, bool __exit) {
    printf("ERROR: %s\n", s);
    if (__exit) _exit(1);
}

//Seperates the commandline for storage
int parseline(char *cmdline, char **argv) {
    int count = 0;
    char *separator = " \n\t";
    argv[count] = strtok(cmdline, separator);

    while ((argv[count] != NULL) && (count + 1 < MAXARGS)) {
        argv[++count] = strtok((char *) 0, separator);
    }

    return count;
}