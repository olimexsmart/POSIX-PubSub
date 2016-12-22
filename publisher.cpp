/*
    Publisher
    This process simply reads a text file with random speed writing
    the text into a given pipe.
    Since this code will be the same for all pulishers, it needs
    to take as arguments the pipe and the file name.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>

#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main(int argc, char const *argv[]) {

    printf(ANSI_COLOR_GREEN "PUBLISHER: Starting...\n" ANSI_COLOR_RESET);
    if(argc != 2) {
        printf(ANSI_COLOR_GREEN "PUBLISHER: Number of arguments not correct.\n" ANSI_COLOR_RESET);
        return -1;
    }

    int writingPipe = atoi(argv[0]);
    char const* const fileName = argv[1];
    char line[256];

    struct timeval time;
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

    FILE* file = fopen(fileName, "r");
    if(file == NULL) {
        printf(ANSI_COLOR_GREEN "PUBLISHER-%d: Could not open file.\n" ANSI_COLOR_RESET, getpid());
        return -1;
    }

    //  Wait to other structures to become ready
    sleep(3);

    while (fgets(line, sizeof(line), file)) {
        int n = write(writingPipe, line, strlen(line));
        printf(ANSI_COLOR_GREEN "PUBLISHER-%d: Wrote %d characters.\n" ANSI_COLOR_RESET, getpid(), n);
        sleep(rand() % 2); //   Sleep for some random time, between 0 and 5 seconds
    }

    fclose(file);
    // for(int i = 0; i < argc; i++)
    //     free(argv[i]);
    // free(argv);

    printf(ANSI_COLOR_GREEN "PUBLISHER-%d: Reached end of the file. Exiting.\n" ANSI_COLOR_RESET, getpid());

    return 0;
}
