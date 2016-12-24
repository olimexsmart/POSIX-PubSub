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

echo "SCRIPT: Make sure that you have color enabled in your terminal!"
echo "SCRIPT: Launching in: (THIS WILL BE FAST, SOMETHING LIKE 6-7 SECONDS OF PURE MADNESS)"
secs=$((5))
while [ $secs -gt 0 ]; do
   echo -ne "SCRIPT: $secs\033[0K\r"
   sleep 1
   : $((secs--))
done

echo "SCRIPT: Launching LAUNCHER..."
./launcher.o publisherA.o publisherB.o mediator.o subscriberA.o subscriberB.o subscriberC.o Inferno.txt Paradiso.txt
echo "SCRIPT: If you see this I would say that we are done."
