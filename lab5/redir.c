/* Program to demo a dup2 system call for I/O redirection */
#include <sys/stat.h>     
#include <fcntl.h>
#include <stdio.h>    
int main (int argc, char *argv[])
{
  if (argc == 1) { printf("Usage: redir <filename.txt>\n"); return 0; }
  int fileId = open(argv[1],O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if( fileId < 0 )  {  
     printf( "error creating file \n" ); 
     return (1);     
  }
  printf("About to run ls\n");
  dup2( fileId, 1); /* copy fileID to stdout */
  close( fileId ); /* close fileId - no longer need */     		
  char *cmd[] = {"ls",0 };
  execvp(cmd[0], cmd);
  return 0;
}
