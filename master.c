#include "config.h"

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

void cleanup_shm(int nproc, int * segIDs)
{
    for (int i = 0; i < nproc; i++)
    {
        shmctl(segIDs[i], IPC_RMID, 0);
    }
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
    for (int i = 0; i < n; i++)
    {
        choosing[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        number[i] = 0;
    }
    //Allocate shared memory
    for (int i = 0; i < n; i++)
    {
        choosing[i] = shmget(IPC_PRIVATE, sizeof(bool), IPC_CREAT);
    }
    for (int i = 0; i < n; i++)
    {
        number[i] = shmget(IPC_PRIVATE, sizeof(bool), IPC_CREAT);
    }

    for (int i = 0; i < n - 1; i++)
    {
        //pid = fork();

        if (pid == -1)
        {
            printf("Fork failed, program exiting early...\n");
            error_fork();
        }

        if (pid == 0)
        {
            //this is what the child will do
            //execl("./slave", itoa(i)); // replaces the child fork with an instance of child, with its process number passed to it
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
    for (int i = 0; i < n; i++)
    {
        shmctl(choosing[i], IPC_RMID, 0);
    }
    cleanup_shm(n, number);

    exit(0);
}