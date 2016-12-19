/*
    Test bench for the class hierarcy
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>

#include "Topic.h"

int main(int argc, char const *argv[]) {

	printf("Starded\n");
    Topic bsoul(56, 0, 4);

	printf("Ended\n");
    return 0;
}
