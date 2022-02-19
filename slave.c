#include "config.h"

int main(int argc, char * argv[])
{
    int proc_id = atoi(argv[1]);

    int * sh_mem = shmat(proc_id, NULL, 0);

    char fileName[11] = "logfile.";
    strncat(fileName, (char)proc_id, 2);

    FILE *logfile = fopen(fileName, "w");

    if (logfile == NULL)
    {
        printf("Error, logfile didn't open\n");
        exit(-1);
    }

    for (int i = 0; i < 5; i++)
    {
        //Do bakery algorithm stuff to protect critical section
        
        //Write to file
        fprintf(logfile, "Writing to logfile\n");      
        //More bakery algorithm stuff
    }

    shmdt(sh_mem);
    fclose(logfile);

    return 0;
}