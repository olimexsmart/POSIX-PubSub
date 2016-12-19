/*
  Topic class that manages a internal circular buffer and his subscrption feed
*/

#ifndef TOPIC_H
#define TOPIC_H

#include "Subscriber.h"
#include "Publisher.h"
#include <vector>
#include <stdlib.h>

using namespace std;

class Topic
{
public:
    Topic(pid_t publisherPID, int publisherFDData, int capacity, bool overwrite = true);
    void Subscribe(pid_t subscriberPID, int subscriberFDrequest, int subscriberFDresponse);
    void Unsubscribe(pid_t subscriberPID);

    int TakeData();
    int SendData(pid_t subscriberPID);

    ~Topic();

private:
    //  Circular buffer variables
    char * data;
    int capacity;
    bool overwrite;
    int head;
    //Circuit buffer handlers
    int writeCBuffer(char * buffer, int lenght); //Public function with same use
    int readCBuffer(char * buffer, int lenght, int * tail);

    //  Connection to Process objects
    Publisher * publisher;
    vector<Subriscriber> subscribers;
    vector<int> tails;

    //Hide default constructor
    Topic();
};

#endif
