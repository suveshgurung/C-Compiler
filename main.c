#define _GNU_SOURCE
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>


/* structure to store the lines read */
typedef struct {
    char *line;
    size_t len;
} Read_Characters;

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "Invalid usage\r\n");
        fprintf(stderr, "USAGE : %s <file>\r\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    // Read_Characters charBuf;

    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t length;
    ssize_t numOfCharRead;

    while ((numOfCharRead = getline(&line, &length, fp)) != -1) {
        printf("%c", line[numOfCharRead - 2]);
    }
    printf("\n");

    free(line);

    return 0;
}
