/* Author:  Andrew Peklar
 * Title:   Take Home Final - Option B
 * Teacher: Prof. Nguyen
 * Date:    12/15/2015 
 */

#include "semun.h"
#include "curr_time.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {
    pid_t pid;
    int i, sem_id;
    unsigned int count_children = argc - 1;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sleep-time...\n", argv[0]);

    setbuf(stdout, NULL);
    printf("%s  Parent Process Initialized.\n", currTime("%T"));

    /* create new semaphore: IPC_Private ensures unique ID
       0666 sets permissions */
    sem_id = semget(IPC_PRIVATE, (count_children), 0666);        // sem_id set to get → child account for array of entries 
    printf("Semiphore ID: %d\n", sem_id);                        // Ouput sem_id number

    for (i = 0; i < argc; ++i) 
        semctl(sem_id, i, SETVAL, 0);

    for (i = 0; i < count_children; i++) {

        /* Error catching if -1 value */
        switch (fork()) {
        case -1:                                               
            errExit("fork %d", i);

        /*Child Process stuff here */
        case 0:                                     

            sleep(getInt(argv[i + 1], GN_NONNEG, "sleep-time"));
            printf("%s  Child %d (PID=%ld) child process unlocking.\n", currTime("%T"), i, (long) getpid());

            struct sembuf c_buffer;
            c_buffer.sem_num = i;
            c_buffer.sem_op = 1;  // current semiphore
            c_buffer.sem_flg = 0;

            /* Decriment semaphore here */ 
            if (semop(sem_id, &c_buffer, 1) < 0) {
                perror("semop");
                _exit(EXIT_FAILURE);
            }
            _exit(EXIT_SUCCESS);
       
        /* Parent Process */
        default: 
            break;
        }
    }

    struct sembuf p_buffer[count_children];
    for (i = 0; i < argc; i++) {
        p_buffer[i].sem_op = -1; 
        p_buffer[i].sem_num = i; // Track entries
        p_buffer[i].sem_flg = 0;
    }

    semop(sem_id, p_buffer, count_children);

    printf("%s  Obstacles removed, Parent may proceed .\n", currTime("%T"));

    /* remove semaphore */
    /* Exclude for final_check.sh */
     semctl(sem_id, 0, IPC_RMID, NULL);
     printf("Semiphore id (%d) succesfully removed. \n", sem_id);  

    exit(EXIT_SUCCESS);
}
