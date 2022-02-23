#include "config.h"

//globals
bool * choosing_ptr;
int * numbers_ptr;
int choosing_id, numbers_id;

void oot_handler()
{
    //handler to call the out of time error function 
    error_oot();
}

void cc_handler()
{
    
}

void error_oot()
{ 
    // out of time
    //go through pid list and remove still running children
    exit(-1);
}

void error_fork()
{
    //go thorough the pid list and kill any process that is still running
    //idealy this should never be used
    exit(-1);
}

void cleanup_shm()
{
    //Detach shared memory
    shmdt(choosing_ptr);
    shmdt(numbers_ptr);

    //Remove shared memory
    shmctl(choosing_id, IPC_RMID, NULL);
    shmctl(numbers_id, IPC_RMID, NULL);
}

int main(int argc, char *argv[])
{
    //This signal catches the alarm if time runs out
    signal(SIGALRM, oot_handler);
    signal(SIGINT, cc_handler);

    int pid, opt, ss = 100, n = 20, max_sec = 100, max_n = 20;

    int * pid_list = malloc(sizeof(int) * n);

    while ((opt = getopt(argc, argv, "t:")) != -1)
    {
        switch (opt)
        {
        case 't':
            ss = atoi(optarg); // sets the max number of seconds the master will wait for the children to terminate normally
            break;
        default:
            printf("Error: invalid argument, exiting...\n\n");
            exit(-1);
        }
    }

    if (argc != 4)
    {
        printf("Incorrect call of master. Try ./master -t (Time) (Number of Processes)\n");
    }
    else
    {
        n = atoi(argv[3]);
        //printf("N is: %d\n", n);
        if (n > max_n)
        {
            printf("N was too large and was set to default value of 20\n");
            n = max_n;
        }
    }
   
    bool * choosing = malloc(sizeof(bool) * n);
    int * number = malloc(sizeof(int) * n);
    //Initialize both arrays
    key_t choosing_key = ftok("Makefile", 'a');
    choosing_id = shmget(choosing_key, sizeof(bool) * 20, IPC_CREAT | 0666);

    key_t numbers_key = ftok(".", 'a');
    numbers_id = shmget(numbers_key, sizeof(int) * 20, IPC_CREAT | 0666);

    if (choosing_id == -1){
        perror("monitor.c: Error: Shared memory (buffer) could not be created");
        printf("exiting\n\n");
        //early cleanup
        exit(0);
    }

    if (numbers_id == -1){
        perror("monitor.c: Error: Shared memory (buffer) could not be created");
        printf("exiting\n\n");
        //early cleanup
        exit(0);
    }

    //shm has been allocated, now we can attach
    choosing_ptr = (bool *)shmat(choosing_id, 0, 0);
    numbers_ptr = (int *)shmat(numbers_id, 0, 0);

    for (int i = 0; i < n; i++)
    {
        choosing_ptr[i] = false;
    }
    for (int i = 0; i < n; i++)
    {
        numbers_ptr[i] = 0;
    }
    
    for (int i = 0; i < n - 1; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            printf("Fork failed, program exiting early...\n");
            error_fork();
        }

        if (pid == 0)
        {
            char * pnum = malloc(sizeof(char) * 3);
            sprintf(pnum, "%d", i + 1);
            //this is what the child will do
            execl("./slave", "./slave", pnum, NULL); // replaces the child fork with an instance of child, with its process number passed to it
        }
        else
        {
            //this is what the parent will do
            pid_list[i] = pid; //add pid to a pid list
        }
    }

    //after all of the children are forked setup a signal handler for the alarm
    alarm(ss);

    wait(NULL); // wait for all the children to exit

    //Clean shared memory for choosing, then for number
    cleanup_shm();

    exit(0);
}