/*
    Publiscer class that inherits from Process
*/

#ifndef PUBLISHER_H
#define PUBLISHER_H

class Publischer : public Process
{
public:
    Publischer(pid_t PID, int FDData) : Process(PID)
    {
        this.FDData = FDData;
    }

    int GetFDData() { return FDData; }

private:
    int FDData;
}

#endif
