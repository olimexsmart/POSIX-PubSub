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
#include <fcntl.h>
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
    if(argc != 9) {
        printf(ANSI_COLOR_YELLOW "The convention here is to use the first input argument as the mediator executable. Then the two publishers, the mediator and at the and the three subscribers, then followed by the two text file names.\n" ANSI_COLOR_RESET);
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
    if( pipe2(publisherAPipe, O_NONBLOCK) == -1 || pipe2(publisherBPipe, O_NONBLOCK) == -1 || pipe2(subscriberCReqPipe, O_NONBLOCK) == -1 ||
        pipe2(subscriberAReqPipe, O_NONBLOCK) == -1 || pipe2(subscriberBReqPipe, O_NONBLOCK) == -1 || pipe2(subscriberCDataPipe, O_NONBLOCK) == -1 ||
        pipe2(subscriberADataPipe, O_NONBLOCK) == -1 || pipe2(subscriberBDataPipe, O_NONBLOCK) == -1) {
        perror("Could not open all the necessary pipes. Exiting.\n");
        return -1;
    }

    /*
        now print some dbug information,
        then start the scheleton of the forking process
    */

    printf(ANSI_COLOR_YELLOW "LAUNCHER: Names and pipes correctly processed. Starting fork processes\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_YELLOW "LAUNCHER: PID: %d\n" ANSI_COLOR_RESET, getpid());

    //  ------->    PublisherA forking
    publisherA = fork();
    if(publisherA < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(publisherA == 0) {
        printf(ANSI_COLOR_GREEN "PUBLISHER-A: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

        char ** arguments = (char**) malloc(sizeof(char*) * 3);
        for(int i = 0; i < 2; i ++)
            arguments[i] = (char*) malloc(sizeof(char) * 15);
        arguments[2] = NULL;

        sprintf(arguments[0], "%d", publisherAPipe[1]);
        sprintf(arguments[1], "%s", argv[7]);   //Inferno.txt

        close(publisherAPipe[0]);

        execvp(publisherAName, arguments);

    }

    //  ------->    PublisherB forking
    publisherB = fork();
    if(publisherB< 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(publisherB == 0) {
        printf(ANSI_COLOR_GREEN "PUBLISHER-B: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

        char ** arguments = (char**) malloc(sizeof(char*) * 3);
        for(int i = 0; i < 2; i ++)
            arguments[i] = (char*) malloc(sizeof(char) * 15);
        arguments[2] = NULL;

        sprintf(arguments[0], "%d", publisherBPipe[1]);
        sprintf(arguments[1], "%s", argv[8]);   //Paradiso.txt

        close(publisherBPipe[0]);

        execvp(publisherBName, arguments);
    }

    //  ------->    SubscriberA forking
    subscriberA = fork();
    if(subscriberA < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(subscriberA == 0) {
        printf(ANSI_COLOR_BLUE "SUBSCRIBER-A: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

        char ** arguments = (char**) malloc(sizeof(char*) * 5);
        for(int i = 0; i < 4; i ++)
            arguments[i] = (char*) malloc(sizeof(char) * 15);
        arguments[4] = NULL;

        sprintf(arguments[0], "%d", subscriberAReqPipe[1]);
        sprintf(arguments[1], "%d", subscriberADataPipe[0]);
        sprintf(arguments[2], "%d", publisherA);
        sprintf(arguments[3], "%d", publisherB);

        close(subscriberAReqPipe[0]);
        close(subscriberADataPipe[1]);

        printf(ANSI_COLOR_BLUE "SUBSCRIBER-A: Ready to execvp.\n" ANSI_COLOR_RESET);
        execvp(subscriberAName, arguments);
    }

    //  ------->    SubscriberB forking
    subscriberB = fork();
    if(subscriberB < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(subscriberB == 0) {
        printf(ANSI_COLOR_BLUE "SUBSCRIBER-B: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

        char ** arguments = (char**) malloc(sizeof(char*) * 5);
        for(int i = 0; i < 4; i ++)
            arguments[i] = (char*) malloc(sizeof(char) * 15);
        arguments[4] = NULL;

        sprintf(arguments[0], "%d", subscriberBReqPipe[1]);
        sprintf(arguments[1], "%d", subscriberBDataPipe[0]);
        sprintf(arguments[2], "%d", publisherA);
        sprintf(arguments[3], "%d", publisherB);

        close(subscriberBReqPipe[0]);
        close(subscriberBDataPipe[1]);

        printf(ANSI_COLOR_BLUE "SUBSCRIBER-B: Ready to execvp.\n" ANSI_COLOR_RESET);
        execvp(subscriberBName, arguments);
    }

    //  ------->    SubscriberC forking
    subscriberC = fork();
    if(subscriberC < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(subscriberC == 0) {
        printf(ANSI_COLOR_BLUE "SUBSCRIBER-C: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

        char ** arguments = (char**) malloc(sizeof(char*) * 5);
        for(int i = 0; i < 4; i ++)
            arguments[i] = (char*) malloc(sizeof(char) * 15);
        arguments[4] = NULL;

        sprintf(arguments[0], "%d", subscriberCReqPipe[1]);
        sprintf(arguments[1], "%d", subscriberCDataPipe[0]);
        sprintf(arguments[2], "%d", publisherA);
        sprintf(arguments[3], "%d", publisherB);

        close(subscriberCReqPipe[0]);
        close(subscriberCDataPipe[1]);

        printf(ANSI_COLOR_BLUE "SUBSCRIBER-C: Ready to execvp.\n" ANSI_COLOR_RESET);
        execvp(subscriberCName, arguments);
    }

    //  ------->    Mediator forking
    mediator = fork();
    if(mediator < 0) {
        perror("Could not fork. Exiting.\n");
        return -1;
    } else if(mediator == 0) {
        printf(ANSI_COLOR_RED "MEDIATOR: Forked correctly, PID: %d. Getting argv ready to exec.\n" ANSI_COLOR_RESET, getpid());

        char ** arguments = (char**) malloc(sizeof(char*) * 14);
        for(int i = 0; i < 13; i ++)
            arguments[i] = (char*) malloc(sizeof(char) * 12);
        arguments[13] = NULL;

        sprintf(arguments[0], "%d", publisherA);
        sprintf(arguments[1], "%d", publisherAPipe[0]);
        sprintf(arguments[2], "%d", publisherB);
        sprintf(arguments[3], "%d", publisherBPipe[0]);
        sprintf(arguments[4], "%d", subscriberA);
        sprintf(arguments[5], "%d", subscriberAReqPipe[0]);
        sprintf(arguments[6], "%d", subscriberADataPipe[1]);
        sprintf(arguments[7], "%d", subscriberB);
        sprintf(arguments[8], "%d", subscriberBReqPipe[0]);
        sprintf(arguments[9], "%d", subscriberBDataPipe[1]);
        sprintf(arguments[10], "%d", subscriberC);
        sprintf(arguments[11], "%d", subscriberCReqPipe[0]);
        sprintf(arguments[12], "%d", subscriberCDataPipe[1]);
        //  Closing unnecessary ends
        close(publisherAPipe[1]);
        close(publisherBPipe[1]);
        close(subscriberADataPipe[0]);
        close(subscriberBDataPipe[0]);
        close(subscriberCDataPipe[0]);
        close(subscriberAReqPipe[1]);
        close(subscriberBReqPipe[1]);
        close(subscriberCReqPipe[1]);

        printf(ANSI_COLOR_RED "MEDIATOR: Ready to execvp.\n" ANSI_COLOR_RESET);
        execvp(mediatorName, arguments);
    }

    //  Six waits ensure that this process terminater when all of them exit
    int status;
    waitpid(publisherA, &status, 0);
    waitpid(publisherB, &status, 0);
    printf(ANSI_COLOR_YELLOW "All Publishers exited, waiting one second and then killing the others.\n" ANSI_COLOR_RESET);
    sleep(1);
    kill(subscriberA, SIGKILL);
    kill(subscriberB, SIGKILL);
    kill(subscriberC, SIGKILL);
    kill(mediator, SIGKILL);
    // waitpid(subscriberA, &status, 0);
    // waitpid(subscriberB, &status, 0);
    // waitpid(subscriberC, &status, 0);
    // waitpid(mediator, &status, 0);
    //TODO: IF MEDIATOR FAILS, CLOSE ALL CHILDS

    printf(ANSI_COLOR_YELLOW "LAUNCHER: All child processes terminated, exiting myself(%d).\n Should have been generated six files with the same content of the input ones.\n See you soon. \n" ANSI_COLOR_RESET, getpid());
    return 0;
}
