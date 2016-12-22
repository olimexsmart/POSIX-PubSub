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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>

int main(int argc, char const *argv[]) {

    if(argc != 2) {
        printf(ANSI_COLOR_GREEN "PUBLISHER: Number of arguments not correct.\n" ANSI_COLOR_RESET);
        return -1;
    }

    int writingPipe = atoi(argv[0]);
    char const* const fileName = argv[1];
    char line[256];
    time_t t;
    srand((unsigned) time(&t));

    FILE* file = fopen(fileName, "r");
    if(file == NULL) {
        printf(ANSI_COLOR_GREEN "PUBLISHER-%s: Could not open file.\n" ANSI_COLOR_RESET, argv[1]);
        return -1;
    }


    while (fgets(line, sizeof(line), file)) {
        int n = write(writingPipe, line, strlen(line));
        printf(ANSI_COLOR_GREEN "PUBLISHER-%s: Wrote %d characters.\n" ANSI_COLOR_RESET, argv[1], n);
        sleep(rand() % 5); //   Sleep for some random time, between 0 and 5 seconds
    }

    fclose(file);

    printf(ANSI_COLOR_GREEN "PUBLISHER-%s: Reached end of the file. Exiting.\n" ANSI_COLOR_RESET, argv[1]);

    return 0;
}
