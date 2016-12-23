#!/bin/bash

echo "SCRIPT: Started compiling..."
g++ launcher.cpp -o launcher.o
echo "SCRIPT: Launcher compiled."
g++ mediator.cpp Topic.cpp -o mediator.o
echo "SCRIPT: Mediator compiled."
g++ publisher.cpp -o publisherA.o
g++ publisher.cpp -o publisherB.o
echo "SCRIPT: Publishers compiled."
g++ subscriber.cpp -o subscriberA.o
g++ subscriber.cpp -o subscriberB.o
g++ subscriber.cpp -o subscriberC.o
echo "SCRIPT: Subscribers compiled."

echo "Launching in:"
secs=$((5))
while [ $secs -gt 0 ]; do
   echo -ne "$secs\033[0K\r"
   sleep 1
   : $((secs--))
done

echo "SCRIPT: Launching LAUNCHER..."
./launcher.o publisherA.o publisherB.o mediator.o subscriberA.o subscriberB.o subscriberC.o Inferno.txt Paradiso.txt
echo "SCRIPT: If you see this we are done I would say."
