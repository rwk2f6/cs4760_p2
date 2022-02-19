#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>
#include <getopt.h>

#define N 15

enum {SEGMENT_SIZE = 0x6400};

int * allocateSMem(int);
void deallocateSMem(int, int *);
void oot_handler();
void error_oot();
void error_fork();