To compile:

gcc pipe_sync.c get_num.c curr_time.c error_functions.c -o pipe_sync
 
(One time only. Otherwise, we use makefile)

To execute the pipe.c program:

pipe 2 3 6

pipe 0 0 0 

pipe 0 0 9

pipe 9 4 0

pipe 4 4 
 
Few questions:

1. Which process (parent or child) displays the final text? Is this always the case and why?

2. Which process creates child process(es)? Line number?

