/*
    Launcher program.
    This need to fork several times and initialize, through the exec system call,
    the corrects arguments of all the children processes.
    After doing that waits for all the children processes to complete and then
    exits.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    /*
        The convention here is to use the first input argument
        as the mediator executable. Then the two publishers and at the and the
        three subscribers. The arguments number has to be  6 plus the
        name of the launcher itself, 7 in total.
    */
    if(argc != 7) {
        printf("The convention here is to use the first input argument
        as the mediator executable. Then the two publishers and at the and the
        three subscribers.\n");
        return -1;
    }

    //  These will hold PIDs of the child processes
    pid_t publisherA;
    pid_t publisherB;
    pid_t mediator;
    pid_t subscriberA;
    pid_t subscriberB;
    pid_t subscriberC;
    //  Saving name of the executable files
    char publisherAName[50] = "./\0";
    char publisherBName[50] = "./\0";
    char mediatorName[50] = "./\0";
    char subscriberAName[50] = "./\0";
    char subscriberBName[50] = "./\0";
    char subscriberCName[50] = "./\0";
    strcat(publisherAName, argv[1]);
    strcat(publisherBName, argv[2]);
    strcat(mediatorName, argv[3]);
    strcat(subscriberAName, argv[4]);
    strcat(subscriberBName, argv[5]);
    strcat(subscriberCName, argv[6]);

    //  Opening all the necessary pipes
    int publisherAPipe[2];          //  Publishers need only to send data
    int publisherBPipe[2];
    int subscriberADataPipe[2];     //  Subscripers need two pipes:
    int subscriberAReqPipe[2];      //  one to send the request
    int subscriberBDataPipe[2];     //  the other to retreive the data
    int subscriberBReqPipe[2];
    int subscriberCDataPipe[2];
    int subscriberCReqPipe[2];
    if( pipe(publisherAPipe) == -1 || pipe(publisherBPipe) == -1 || pipe(subscriberCReqPipe) == -1 ||
        pipe(subscriberAReqPipe) == -1 || pipe(subscriberBReqPipe) == -1 ||pipe(subscriberCDataPipe) == -1 ||
        pipe(subscriberADataPipe) == -1 || pipe(subscriberBDataPipe) == -1 ||) {
        perror("Could not open all the necessary pipes. Exiting.\n");
        return -1;
    }

    /*
        now print some dbug information,
        then start the scheleton of the forking process
    */




    return 0;
}
