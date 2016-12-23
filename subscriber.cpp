/*
    Publisher
    Since this code will be compiled for each subscriber,
    it must take af arguments its request and data pipes.
    His job is to send requests at random time to random
    publishers. The mediator will raise a opportune signal
    when data is ready to read. A signal handler function
    will read the data and write it on a file.
    The third argument is a name assigned to the subscriber
    by the Launcher program, in order to disnguish them
    during execution.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>

#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void HandleSIGIO(int signal);

//  Some global vars used into the HandleSIGIO function
int receiveFD;

int main(int argc, char const *argv[]) {

    printf(ANSI_COLOR_BLUE "SUBSCRIBER: Starting...\n" ANSI_COLOR_RESET);

    if(argc < 4) {  //  At least one publisher
        printf(ANSI_COLOR_BLUE "SUBSCRIBER: Number of arguments not correct.\n" ANSI_COLOR_RESET);
        return -1;
    }

    //  Just better names and sharing data to the IRQ routine
    int requestFD = atoi(argv[0]);
    receiveFD = atoi(argv[1]);
    char buffer[6] = "";    //  Buffer that will contain the PID of the publisher we want to make to

    //  Structure that holds all topic information
    int Ntopics = argc - 2;
    int * topics = (int *) malloc(sizeof(int) * Ntopics); // Holds the list of topics we are subscribed to
    for(int i = 0; i < Ntopics; i++) {
        topics[i] = atoi(argv[i + 2]);

        //  Create empty files were the data will be written
        FILE *fp;
        char filename[30];
        sprintf(filename, "Subscriber-%d - From %d.data", getpid(), topics[i]);
        fp = fopen (filename, "w");
        if (fp!=NULL)
        {
            fprintf(fp,"%s\n\n", filename);
            fclose (fp);
        }
    }

    //  IRQ attach
    signal(SIGIO, HandleSIGIO);

    //  More random seed
    struct timeval time;
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

    sleep(2); //Wait until mediator is ok

    //  Main loop
    printf(ANSI_COLOR_BLUE "SUBSCRIBER-%d: Started sending requests.\n" ANSI_COLOR_RESET, getpid());
    while (true) {
        sprintf(buffer, "%d", topics[rand() % Ntopics]);    //  Put request PID into the buffer
        int n = write(requestFD, buffer, 6);
        printf(ANSI_COLOR_BLUE "SUBSCRIBER-%d: Sent request->%s to %s.\n" ANSI_COLOR_RESET, getpid(), (n == 6) ? "OK" : "FAILED", buffer);

        //  Take a breath
        struct timespec t, to;
        t.tv_sec = 0;
        t.tv_nsec = (rand() % 75) * 1000000 + 1;
        nanosleep(&t, &to);
        nanosleep(&to, NULL); // In case the pause got interrupted
    }


    free(topics);
    // for(int i = 0; i < argc; i++)
    //     free(argv[i]);
    // free(argv);

    return 0;
}


void HandleSIGIO(int signal)
{
    //  Block SIGIO until finished
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGIO);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    if(signal == SIGIO) {
        //Actual reading, splitting the data in substring since there could be a double entry
        char buffer[1024] = {'\0'};

        int bytesRead = read(receiveFD, buffer, 1023);
        //if(bytesRead <= 6) return; //Should at least carry one char

        char senderPID[6] = "";
        strncpy(senderPID, buffer, 5);
        senderPID[5] = '\0';

        //Writing the data on file
        FILE *fp;
        char filename[35];
        sprintf(filename, "Subscriber-%d - From %s.data", getpid(), senderPID);
        fp = fopen (filename, "a");     //  Append writing mode
        if (fp!=NULL)
        {
            fprintf(fp,"%s", buffer + 6);   //  Avoiding the first 6 characters wich are the preamble
            fclose (fp);
        }

        printf(ANSI_COLOR_BLUE "SUBSCRIBER-%d: Received %d bytes from %s.\n" ANSI_COLOR_RESET, getpid(), bytesRead - 6, senderPID);

    }

    //UNBLOCK SIGIO
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGIO);
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
}
