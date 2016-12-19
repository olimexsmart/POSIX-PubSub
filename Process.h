/*
    Base class to relate different types of objects
*/

#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>
#include <sys/types.h>

using namespace std;

class Process
{
public:
    //Proprieties
    pid_t GetPID() { return PID; }
	
protected:
    //Constructor
    Process(pid_t PID) { this->PID = PID; }
	Process() { PID = 0; }
private:    
    pid_t PID; //Once set this can't be changed
};

#endif
