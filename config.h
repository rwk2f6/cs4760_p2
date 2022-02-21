#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/ipc.h>
#include <errno.h>

int * allocateSMem(int);
void deallocateSMem(int, int *);
void oot_handler();
void cc_handler();
void error_oot();
void error_fork();
int maxValue(int *, int);