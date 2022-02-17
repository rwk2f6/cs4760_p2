#include "config.h"

int main(int argc, char *argv[])
{
    int i, r;

    for (i = 0; i < 5; i++)
    {
        //Do bakery algorithm stuff to protect critical section
        r = rand(5);
        sleep(r);
        //Write to file
        r = rand(5);
        sleep(r);
        //More bakery algorithm stuff
    }

    return 0;
}