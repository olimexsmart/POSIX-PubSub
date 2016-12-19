/*
    Publiscer class that inherits from Process
*/

#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "Process.h"

using namespace std;

class Publisher : public Process
{
public:
    Publisher(pid_t PID, int FDData) : Process(PID) { this->FDData = FDData; }

    int GetFDData() { return FDData; }

private:
    int FDData;
};

#endif
