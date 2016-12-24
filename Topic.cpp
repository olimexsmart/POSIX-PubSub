/*
  Topic class that manages a internal circular buffer and his subscrption feed
*/

#include "Topic.h"

Topic::Topic(pid_t publisherPID, int publisherFDData, int capacity, bool overwrite /*=true*/)
{
    publisher = new Publisher(publisherPID, publisherFDData); //Instance of the publisher of this Topic
    //Init of circular buffer parametres
    data = (char *)malloc(sizeof(char) * capacity);
    head = 0;
    this->capacity = capacity;
    this->overwrite = overwrite;
    tails.clear();
}

Topic::Topic()
{

}

Topic::~Topic()
{
    free(data);
	delete(publisher);
}

pid_t Topic::GetPublisherPID()
{
    return publisher->GetPID();
}

int Topic::writeCBuffer(char * buffer, int lenght)
{
	int i;
    for(i = 0; i < lenght; i++)
    {   //  If not overwriting, stop writing when in frot of you there is a tail pointer
        if(!overwrite)
            for(unsigned int k = 0; k < tails.size(); k++)
                if(tails[k] == ((head + 1) % capacity))
                    return i;

        //  Writing the data and moving the head pointer
        data[head] = buffer[i];
        head++;
        head %= capacity;
        //  If overwriting, when superimposing with a tail pointer move it one position away
        if(overwrite)
            for(unsigned int k = 0; k < tails.size(); k++)
                if(head == tails[k]) {
                    tails[k] = (tails[k] + 1) % capacity;    //Overwrite data dragging them along
                    printf(ANSI_COLOR_CYAN "TOPIC-%d: Overwriting data of %d #########################################.\n" ANSI_COLOR_RESET, this->GetPublisherPID(), subscribers[k].GetPID());
                }
    }
    return i;
}


int Topic::readCBuffer(char * buffer, int lenght, int * tail)
{   //Valid data are one cell before the head pointer
	int i;
    for (i = 0; i < lenght; i++) {
        if(*tail == head)  //We have no more data to read
            return i;
        //  Read and move the tail pointer
        buffer[i] = data[*tail];
        (*tail)++;
        (*tail) %= capacity;
    }

    return i;
}

void Topic::Subscribe(pid_t subscriberPID, int subscriberFDrequest, int subscriberFDresponse)
{
    //Create new Instance of a subscriber
    Subriscriber s(subscriberPID, subscriberFDrequest, subscriberFDresponse);
    subscribers.push_back(s);   //Copy it at the end of the array
    tails.push_back(head);      //For a new subscriber consider valid data only from now on
}

void Topic::Unsubscribe(pid_t subscriberPID)
{
    for(unsigned int k = 0; k < subscribers.size(); k++)
        if(subscribers[k].GetPID() == subscriberPID)
        {
            subscribers.erase(subscribers.begin() + k);
            tails.erase(tails.begin() + k);
        }
}

int Topic::SendData(pid_t subscriberPID)
{
    //First of all, find the subscriber
    for(unsigned int k = 0; k < subscribers.size(); k++)
        if(subscribers[k].GetPID() == subscriberPID)
        {
            char buffer[capacity];
            //printf(ANSI_COLOR_CYAN "TOPIC-%d: Tail of %d at %d before reading.\n" ANSI_COLOR_RESET, this->GetPublisherPID(), subscribers[k].GetPID(), tails[k]);
            int bytes = readCBuffer(buffer, capacity, &tails[k]);
            //printf(ANSI_COLOR_CYAN "TOPIC-%d: Tail of %d at %d after reading.\n" ANSI_COLOR_RESET, this->GetPublisherPID(), subscribers[k].GetPID(), tails[k]);

            if(bytes != 0) {
                //  Sending premble with the sender's PID
                char senderPID[6] = "";
                sprintf(senderPID, "%d", this->GetPublisherPID());
                write(subscribers[k].GetFDResponse(), senderPID, 6);
                //  Sending the actual data
                write(subscribers[k].GetFDResponse(), buffer, bytes);
                kill(subscribers[k].GetPID(), SIGIO); //Signal that I/O is now possible
                //printf(ANSI_COLOR_CYAN "TOPIC-%d: Sent %d chars to %d.\n" ANSI_COLOR_RESET, this->GetPublisherPID(), bytes, subscribers[k].GetPID());
                return bytes;
            } else {
                //printf(ANSI_COLOR_CYAN "TOPIC-%d: Nothing to send to %d.\n" ANSI_COLOR_RESET, this->GetPublisherPID(), subscribers[k].GetPID());
                return 0;
            }
        }

    return -1; //   If no subscriber with such name
}

int Topic::TakeData()
{
    //  If we are here we are sure that data is available to read
    char buffer[capacity];
    int bytes = read(publisher->GetFDData(), buffer, capacity);
    writeCBuffer(buffer, bytes);
    //printf(ANSI_COLOR_CYAN "TOPIC-%d: Received %d chars\n" ANSI_COLOR_RESET, this->GetPublisherPID(), bytes);
    return bytes;
}
