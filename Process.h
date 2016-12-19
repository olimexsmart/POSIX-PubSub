/*
    Base class to relate different types of objects
*/

#ifndef PROCESS_H
#define PROCESS_H

using namespace std;

class Process
{

protected:
    //Constructor
    Process(pid_t PID) { this->PID = PID; }
    //Proprieties
    pid_t GetPID() { return PID; }

private:
    Process();
    pid_t PID; //Once set this can't be changed
};

#endif
