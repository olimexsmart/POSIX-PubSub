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
