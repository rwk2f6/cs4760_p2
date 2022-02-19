#include "config.h"

int main(int argc, char *argv[])
{
    //Signal function for Bakery Algorithm
    //signal(SIGALRM, oot_handler);

    int pid, n, opt, ss, max_time = 100, max_n = 20;

    int * pid_list = malloc(sizeof(int) * n);

    while ((opt = getopt(argc, argv, "t:p:"))!= -1)
    {
        switch (opt)
        {
            case 't':
                ss = atoi(optarg);
                break;
            case 'p':
                n = atoi(optarg);
                if (n > max_n)
                {
                    n = 20;
                    printf("N value was too large, defaulted to 20\n");
                }
                break;
            default:
                fprintf(stderr, "Unknown option provided, exiting...\n");
                exit(-1);
        }
    }

    //Weird error, argc says its 4 for a call like ./master -t 10 5 but then the if statement below still happens
    //If I remove the exit(1) it seg faults
    // printf("Argc: %d\n", argc);

    // if (argc !=  1)
    // {
    //     fprintf(stderr, "No parameter provided for n! Try './master -t 'Time' 'Num of Processes''\n");
    //     exit(1);
    // }

   // n = atoi(argv[1]);
    // if (n > 20)
    // {
    //     printf("Value was too large, input was changed to 20\n");
    //     n = 20;
    // }
    // printf("Command line parameter recieved: %d\n", n);
    // printf("Getopt t- recieved: %d\n", ss);

    int * segIDs = allocateSMem(N);
    for(int i = 0; i < n; i++)
    {
        printf("Segment ID: %d\n", segIDs[i]);
    }

    for (int j = 0; j < n - 1; j++)
    {
        pid = fork();

        if (pid == -1)
        {
            printf("Fork failed, exiting...\n");
            error_fork();
        }

        if (pid == 0)
        {
            //The child will run this process
            execl("./slave", itoa(j));
        }
        else
        {
            //Parent process runs this
            pid_list[j] = pid;
        }
    }

    alarm(max_time);

    deallocateSMem(N, segIDs);

    return 0;
}

int * allocateSMem(int nproc)
{
    int *segmentIDs = malloc(sizeof(int) * nproc);
    for (int i = 0; i < nproc; i++)
    {
        segmentIDs[i] = shmget(IPC_PRIVATE, SEGMENT_SIZE, IPC_CREAT);
    }
    return segmentIDs;
}

void deallocateSMem(int nproc, int * segIDs)
{
    for (int i = 0; i < nproc; i++)
    {
        shmctl(segIDs[i], IPC_RMID, 0);
    }
}

void error_fork()
{
    //Kill child processes
    exit(-1);
}

void oot_handler()
{

}

void error_oot()
{

}