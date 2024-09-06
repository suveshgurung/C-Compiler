#define _GNU_SOURCE
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

/* structure to store the lines read */
typedef struct {
    char *line;
    size_t len;
} Read_Characters;

#define INIT_RC_BUF { NULL, 0 }

void appendLine(Read_Characters *buf, const char *str, ssize_t length);

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "Invalid usage\r\n");
        fprintf(stderr, "USAGE : %s <file>\r\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    Read_Characters charBuf = INIT_RC_BUF;

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t length;
    ssize_t numOfCharRead;

    while ((numOfCharRead = getline(&line, &length, fp)) != -1) {
        appendLine(&charBuf, line, numOfCharRead);
    }

    printf("%s", charBuf.line);

    free(line);
    fclose(fp);

    return 0;
}


/* functions definition*/

void appendLine(Read_Characters *buf, const char *str, ssize_t length) {
    char *temp = (char *)realloc(buf->line, (buf->len + (size_t)length) + 1);

    if (temp == NULL) {
        perror("realloc");
        return;
    }

    buf->line = temp;
    memcpy(&buf->line[buf->len], str, (size_t)length);

    buf->len += length;

    buf->line[buf->len] = '\0';
}
