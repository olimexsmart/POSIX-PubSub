/*
    Mediator
    This takes data from publisher processes, saving them information
    the structure offered by the class::Topic and answers requests of subscribers
    sending them data from the publishers.
*/

#include <sys/types.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>

#include "Topic.h"

#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//  Very case-specific function to avoid code repetition and readbility
int HandleRequest(pid_t subscriberPID, int subscriberReqPipe, Topic * inferno, Topic * paradiso);


int main(int argc, char const *argv[]) {
    /*
        First thing to do is take data from the argv and initialize
        the Topic Instances, one for each publisher.
        Sadly this initialization is hard-coded and not dynamic.
        This was not the goal of the assignement, but with come conventions
        it can be made without great expense.
    */
    if(argc != 13) {
        printf(ANSI_COLOR_RED "MEDIATOR: Not enough arguments for hard-coded initialization\n" ANSI_COLOR_RESET);
        return -1;
    }
    //  Stuff for publisherA
    //  publisherAPID, publisherAFDData
    Topic Inferno(atoi(argv[0]), atoi(argv[1]), 1024);
    //  Stuff for publisherB
    //  publisherBPID, publisherBFDData
    Topic Paradiso(atoi(argv[2]), atoi(argv[3]), 1024);

    //  Subscribing subscribers to alle publishers
    //  subscriberAPID, subscriberAReqPipe, subscriberADataPipe
    Inferno.Subscribe(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    Paradiso.Subscribe(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    //  subscriberBPID, subscriberBReqPipe, subscriberBDataPipe
    Inferno.Subscribe(atoi(argv[7]), atoi(argv[8]), atoi(argv[9]));
    Paradiso.Subscribe(atoi(argv[7]), atoi(argv[8]), atoi(argv[9]));
    //  subscriberCPID, subscriberCReqPipe, subscriberCDataPipe
    Inferno.Subscribe(atoi(argv[10]), atoi(argv[11]), atoi(argv[12]));
    Paradiso.Subscribe(atoi(argv[10]), atoi(argv[11]), atoi(argv[12]));

    printf(ANSI_COLOR_RED "MEDIATOR: Initialization of the Topic structure completed.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED "MEDIATOR: Starting I/O operations.\n" ANSI_COLOR_RESET);

    fd_set rfds;    //  Structure to hold file descriptors to monitor
    int nfds = -1;  //  Now we have to find the highest value number file descriptors
    if(atoi(argv[1]) > nfds) nfds = atoi(argv[1]);      //  pubA data
    if(atoi(argv[3]) > nfds) nfds = atoi(argv[3]);      //  pubB data
    if(atoi(argv[5]) > nfds) nfds = atoi(argv[5]);      //  subA request
    if(atoi(argv[8]) > nfds) nfds = atoi(argv[8]);      //  subB request
    if(atoi(argv[11]) > nfds) nfds = atoi(argv[11]);    //  subC request
    nfds++;         //  Following the Linux manual

    //  Main Mediator loop
    while (true) {
        FD_ZERO(&rfds);
        FD_SET(atoi(argv[1]), &rfds);
        FD_SET(atoi(argv[3]), &rfds);
        FD_SET(atoi(argv[5]), &rfds);
        FD_SET(atoi(argv[8]), &rfds);
        FD_SET(atoi(argv[11]), &rfds);

        select(nfds, &rfds, NULL, NULL, NULL);
        printf(ANSI_COLOR_RED "MEDIATOR: Some data received.\n" ANSI_COLOR_RESET);

        /*
            This part could be done better.
            Putting all topics into a vector and then loop through them.
            The request contains the PID of the publisher, acting effectively as a topic name.
            This information could be compared with each topic publisher PID, via the propriety GetPublisherPID().
            Now that the correct Topic was found, looping through its list of subscribers
            until the corresponding subscriber is found, and send him the data.
            Also this would be a nice addition to code scalability since the
            the number of topics could be dynamic. Other than being non requested,
            this would require having public access to the subscribers structure that
            each Topic object contains. This would not be semantically correct. An overload of
            the [] operator could solve this problem, but this a story for another time.
        */

        //  Get publisherA data
        if(FD_ISSET(atoi(argv[1]), &rfds)) {
            Inferno.TakeData();
        }

        //  Get publisherB data
        if(FD_ISSET(atoi(argv[3]), &rfds)) {
            Paradiso.TakeData();
        }

        //  Get subscriberA request and send to it the data
        if(FD_ISSET(atoi(argv[5]), &rfds)) {
            HandleRequest(atoi(argv[4]), atoi(argv[5]), &Inferno, &Paradiso);
        }

        //  Get subscriberB request and send to it the data
        if(FD_ISSET(atoi(argv[8]), &rfds)) {
            HandleRequest(atoi(argv[7]), atoi(argv[8]), &Inferno, &Paradiso);
        }

        //  Get subscriberC request and send to it the data
        if(FD_ISSET(atoi(argv[11]), &rfds)) {
            HandleRequest(atoi(argv[10]), atoi(argv[11]), &Inferno, &Paradiso);
        }

    }

    // for(int i = 0; i < argc; i++)
    //     free(argv[i]);
    // free(argv);

    return 0;
}

//  Very case-specific function to avoid code repetition and readbility
int HandleRequest(pid_t subscriberPID, int subscriberReqPipe, Topic * inferno, Topic * paradiso)
{
    //  Getting Topic's PID
    char buffer[5] = "";
    pid_t request;
    read(subscriberReqPipe, buffer, 5);
    request = atoi(buffer);

    if(inferno->GetPublisherPID() == request)
        inferno->SendData(subscriberPID);
    else if(paradiso->GetPublisherPID() == request)
        paradiso->SendData(subscriberPID);
    else
        printf(ANSI_COLOR_RED "MEDIATOR: Could not find any publisher with PID: %d.\n" ANSI_COLOR_RESET, request);
}
