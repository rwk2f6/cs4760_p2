#include "config.h"

int maxValue(int * array, int arraySize)
{
    int max = 0;
    for (int i = 0; i < arraySize; i++)
    {
        if (array[i] >= max)
        {
            max = array[i];
        }
    }
    return max;
}

int main(int argc, char * argv[])
{
    int proc_id = atoi(argv[1]); // gets the process number
    //Initialize time variables to output Systemtime in logfile
    time_t T = time(NULL);
    struct tm tm = *localtime(&T);
    //Initialize srand for random sleep times
    srand(time(NULL));
    int r = 0;

    //open child specific logfile ex. logfile.01, logfile.02 ... logfile.20 etc.
    //Bruh I can't figure this out
    char fileName[] = "logfile.";
    strncat(fileName, (char)proc_id, 3);
    FILE * logfile = fopen(fileName, "w");
    //Where should I open cstest?
    FILE * cstest = fopen("cstest", "w");
    int numberLeng = sizeof(number) / sizeof(int);

    if (logfile == NULL)
    {
        printf("Error openning logfile, exiting...\n");
        exit(-1);
    }
    
    //connect to the shared memory setup by the master
    //this shared memory is used for the communication between processes
    int * sh_memNum = shmat(proc_id, NULL, 0);
    int * sh_memBool = shmat(proc_id, NULL, 0);

    for (int i = 0; i < 5; i++)
    { // loop with value of 5
        //loop will make it so each child will access the shared logfile 5 times or whatever you set this loop to
        do
        {
            choosing[proc_id] = 1;
            number[proc_id] = 1 + maxValue(number, numberLeng);
            choosing[proc_id] = 0;
            //Should I use proc_id?
            for (int j = 0; j < proc_id; j++)
            {
                while (choosing[j]);

                while ((number[j] != 0) && (number[j] < number[proc_id]) || (number[j] == number[i] && j < proc_id));
            }
            //Critical Section, open logfile, write to it, and close
            r = (rand() % 5) + 1; //Plus 1 makes sure it never returns 0
            sleep(r);
            fprintf(fileName, "%02d:%02d:%02d Queue %d File modified by process number %d\n", tm.tm_hour, tm.tm_min, tm.tm_sec, number[proc_id], proc_id);
            r = (rand() % 5) + 1;
            sleep(r);

            //Indicates that this process is complete
            number[proc_id] = 0;

            //remainder_section ??
            fclose(logfile);
        }
        while (1);

    }

    exit(0);
}