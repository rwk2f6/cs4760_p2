#include "config.h"

int main(int argc, char *argv[])
{
    int n;

    if (argc != 2)
    {
        fprintf(stderr, "No parameter provided for n! Try './master 1'\n");
        exit(1);
    }

    n = atoi(argv[1]);
    if (n > 20)
    {
        printf("Value was too large, input was changed to 20\n");
        n = 20;
    }
    printf("Command line parameter recieved: %d\n", n);

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