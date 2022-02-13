#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define N 15

enum {SEGMENT_SIZE = 0x6400};

int * allocateSMem(int);
void deallocateSMem(int, int *);