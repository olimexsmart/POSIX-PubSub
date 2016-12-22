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
const char * subscriberName;

int main(int argc, char const *argv[]) {

    if(argc < 4) {  //  At least one publisher
        printf(ANSI_COLOR_BLUE "SUBSCRIBER: Number of arguments not correct.\n" ANSI_COLOR_RESET);
        return -1;
    }

    time_t t;
    srand((unsigned) time(&t));
    int Ntopics = argc - 3;
    int requestFD = atoi(argv[0]);
    receiveFD = atoi(argv[1]);
    subscriberName = argv[2];  //  Arg[2] contains the name assigned to this subscriber
    int * topics = (int *) malloc(sizeof(int) * Ntopics); // Holds the list of topics we are subscribed to
    for(int i = 0; i < Ntopics; i++)
        topics[i] = atoi(argv[i + 3]);

    char buffer[5] = "";    //  Buffer that will contain the PID of the publisher we want to make to

    //  IRQ attach
    signal(SIGIO, HandleSIGIO);

    printf(ANSI_COLOR_BLUE "SUBSCRIBER-%s: Started sending requests.\n" ANSI_COLOR_RESET, argv[2]);

    while (true) {
        sprintf(buffer, "%d", topics[rand() % Ntopics]);    //  Put request PID into the buffer
        int n = write(requestFD, buffer, 5);
        printf(ANSI_COLOR_BLUE "SUBSCRIBER-%s: Sent request->%s to %s.\n" ANSI_COLOR_RESET, argv[2], (n == 5) ? "OK" : "FAILED", buffer);
        sleep(rand() % 3);  //  Take a breath
    }


    free(topics);
    // for(int i = 0; i < argc; i++)
    //     free(argv[i]);
    // free(argv);

    return 0;
}


void HandleSIGIO(int signal)
{
    if(signal == SIGIO) {
        char buffer[256];
        read(receiveFD, buffer, 256);
        //  TODO: ADD PREAMBLE THAT CARRIES SENDERE INFORMATION AND UNPACK IT HERE.
        printf(ANSI_COLOR_BLUE "SUBSCRIBER-%s: Received data: %s.\n" ANSI_COLOR_RESET, subscriberName, buffer);
    }
}
