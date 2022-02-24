#include "config.h"

// Global IDs
bool *choosing_ptr;
int *numbers_ptr;
int choose_id, number_id;

int maxValue(int *array, int arraySize)
{
    int max = 0;
    for (int k = 0; k < arraySize; k++)
    {
        if (array[k] >= max)
        {
            max = array[k];
        }
    }
    return max;
}

int main(int argc, char *argv[])
{
    int nproc = atoi(argv[2]);   // gets the number of processes
    int proc_id = atoi(argv[1]); // gets the process number
    // Attach shared memory
    printf("Inside slave process\n");
    key_t choosing_key = ftok("Makefile", 'a');
    choose_id = shmget(choosing_key, sizeof(bool) * nproc, IPC_EXCL);

    key_t numbers_key = ftok(".", 'a');
    number_id = shmget(numbers_key, sizeof(int) * nproc, IPC_EXCL);

    if (choose_id == -1)
    {
        perror("monitor.c: Error: Shared memory (buffer) could not be created");
        printf("exiting\n\n");
        // early cleanup
        exit(0);
    }

    if (number_id == -1)
    {
        perror("monitor.c: Error: Shared memory (buffer) could not be created");
        printf("exiting\n\n");
        // early cleanup
        exit(0);
    }

    // shm has been gotten
    choosing_ptr = (bool *)shmat(choose_id, 0, 0);
    numbers_ptr = (int *)shmat(number_id, 0, 0);

    // Initialize srand for random sleep times
    srand(proc_id);
    int r = 0;

    // open child specific logfile ex. logfile.01, logfile.02 ... logfile.20 etc.
    char *logname = malloc(sizeof(char) * 11);
    sprintf(logname, "logfile.%d", proc_id);
    FILE *logfile = fopen(logname, "a");
    // open cstest
    FILE *cstest = fopen("cstest", "a");

    if (logfile == NULL)
    {
        printf("Error openning logfile, exiting...\n");
        exit(-1);
    }

    for (int i = 0; i < 5; i++)
    { // loop with value of 5
      // loop will make it so each child will access the shared logfile 5 times or whatever you set this loop to
        choosing_ptr[proc_id] = 1;
        numbers_ptr[proc_id] = 1 + maxValue(numbers_ptr, nproc);
        choosing_ptr[proc_id] = 0;
        for (int j = 0; j < nproc; j++)
        {
            while (choosing_ptr[j]);

            while ( (numbers_ptr[j] != 0) && (numbers_ptr[j] < numbers_ptr[proc_id] || (numbers_ptr[j] == numbers_ptr[proc_id] && j < proc_id)) );
        }
        // Critical Section, open logfile, write to it, and close
        r = (rand() % 5) + 1; // Plus 1 makes sure it never returns 0, range is 1-5
        sleep(r);
        // Initialize time variables to output Systemtime in logfile
        time_t T = time(NULL);
        struct tm tm = *localtime(&T);
        fprintf(cstest, "%02d:%02d:%02d Queue %d File modified by process number %d\n", tm.tm_hour, tm.tm_min, tm.tm_sec, numbers_ptr[proc_id], proc_id);
        fprintf(logfile, "%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
        printf("We made it to the critical section\n");
        r = (rand() % 5) + 1;
        sleep(r);

        // Indicates that this process is complete
        numbers_ptr[proc_id] = 0;
    }
    printf("Exiting Critical Section...\n");
    // remainder_section ??
    fclose(logfile);
    fclose(cstest);

    // Detach shared memory
    shmdt(choosing_ptr);
    shmdt(numbers_ptr);
    // Remove shared memory
    shmctl(choose_id, IPC_RMID, NULL);
    shmctl(number_id, IPC_RMID, NULL);

    exit(0);
}