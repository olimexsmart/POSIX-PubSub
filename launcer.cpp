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

#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main(int argc, char const *argv[]) {
    /*
        The convention here is to use the first input argument
        as the mediator executable. Then the two publishers and at the and the
        three subscribers. The arguments number has to be  6 plus the
        name of the launcher itself, 7 in total.
    */
    if(argc != 7) {
        printf("The convention here is to use the first input argument\
        as the mediator executable. Then the two publishers and at the and the\
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
        pipe(subscriberADataPipe) == -1 || pipe(subscriberBDataPipe) == -1) {
        perror("Could not open all the necessary pipes. Exiting.\n");
        return -1;
    }

    /*
        now print some dbug information,
        then start the scheleton of the forking process
    */

    printf("LAUNCHER: Names and pipes correctly processed. Starting fork processes\n");
	printf("INIT: PID: %d\n", getpid());
    //  ------->    Mediator forking
    mediator = fork();
    if(mediator < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(mediator == 0) {
        printf(ANSI_COLOR_RED "MEDIATOR: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

    }

    //  ------->    PublisherA forking
    publisherA = fork();
    if(publisherA < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(publisherA == 0) {
        printf(ANSI_COLOR_GREEN "PUBLISHER-A: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

    }

    //  ------->    PublisherB forking
    publisherB = fork();
    if(publisherB< 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(publisherB == 0) {
        printf(ANSI_COLOR_GREEN "PUBLISHER-B: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

    }

    //  ------->    SubscriberA forking
    subscriberA = fork();
    if(subscriberA < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(subscriberA == 0) {
        printf(ANSI_COLOR_BLUE "SUBSCRIBER-A: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

    }

    //  ------->    SubscriberB forking
    subscriberB = fork();
    if(subscriberB < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(subscriberB == 0) {
        printf(ANSI_COLOR_BLUE "SUBSCRIBER-B: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

    }

    //  ------->    SubscriberC forking
    subscriberC = fork();
    if(subscriberC < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(subscriberC == 0) {
        printf(ANSI_COLOR_BLUE "SUBSCRIBER-C: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

    }

    //  Six waits ensure that this process terminater when all of them exit
    int status;
    wait(&status);
    wait(&status);
    wait(&status);
    wait(&status);
    wait(&status);
    wait(&status);
    //TODO: IF MEDIATOR FAILS, CLOSE ALL CHILDS
    
    printf("LAUNCHER: All child processes terminated, exiting myself. See you soon. \n");
    return 0;
}
