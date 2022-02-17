#include "config.h"

int main(int argc, char *argv[])
{
    int n, opt, ss;

    while ((opt = getopt(argc, argv, "t:"))!= -1)
    {
        switch (opt)
        {
            case 't':
                ss = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Unknown option provided, exiting...\n");
                exit(-1);
        }
    }

    //Weird error, argc says its 4 for a call like ./master -t 10 5 but then the if statement below still happens
    //If I remove the exit(1) it seg faults
    printf("Argc: %d\n", argc);

    if (argc != 4)
    {
        fprintf(stderr, "No parameter provided for n! Try './master -t 'Time' 'Num of Processes''\n");
        exit(1);
    }

    n = atoi(argv[4]);
    if (n > 20)
    {
        printf("Value was too large, input was changed to 20\n");
        n = 20;
    }
    printf("Command line parameter recieved: %d\n", n);
    printf("Getopt t- recieved: %d\n", ss);

    int * segIDs = allocateSMem(N);
    for(int i = 0; i < N; i++)
    {
        printf("Segment ID: %d\n", segIDs[i]);
    }
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