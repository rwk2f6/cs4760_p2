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

void cleanup_shm();
void oot_handler();
void cc_handler();
void error_cc();
void error_oot();
void error_fork();
int maxValue(int *, int);
void print_choosing(int);
void print_numbers(int);
void cleanup();