/*
    Subriscriber class, inherits his nature from Process class
*/

#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

using namespace std;

class Subriscriber : public Process
{

public:
    Subriscriber(pid_t PID, int FDRequest, int FDResponse) : Process(PID)
    {
        this.FDRequest = FDRequest;
        this.FDResponse = FDResponse;
    }

    int GetFDRequest() { return FDRequest; }
    int GetFDResponse() { return FDResponse; }

private:
    Subriscriber();
    int FDRequest;
    int FDResponse;
};

#endif
