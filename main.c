#define _GNU_SOURCE
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <string.h>
// #include <ctype.h>
#include <stdio.h>

typedef enum {
    KEYWORD,
    STRING,
    SPECIAL_CHARACTER
} Token_List;

/* structure to store the lines read */
typedef struct {
    char *line;
    size_t len;
} Read_Characters;

/* structure which stores the tokens */
typedef struct {
    Token_List tokenType;   
    char *value;
} Token;


#define INIT_RC_BUF { NULL, 0 }

void appendLine(Read_Characters *buf, const char *str, ssize_t length);
void tokenize(const char *str, Token **tokenBuf);

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

    Token *tokenBuf = NULL;
    char *line = NULL;
    size_t length;
    ssize_t numOfCharRead;

    while ((numOfCharRead = getline(&line, &length, fp)) != -1) {
        tokenize(line, &tokenBuf);
        appendLine(&charBuf, line, numOfCharRead);
    }

    printf("%s\n", tokenBuf[0].value);
    printf("%d\n", tokenBuf[0].tokenType);

    if (charBuf.line != NULL) {
        free(charBuf.line);
    } 
    if (tokenBuf != NULL) {
        if (tokenBuf->value != NULL) {
            free(tokenBuf->value);
        }
        free(tokenBuf);
    }
    if (line != NULL) {
        free(line);
    }
    fclose(fp);

    return 0;
}


/* functions definition*/
void appendLine(Read_Characters *buf, const char *str, ssize_t length) {
    char *temp = (char *)realloc(buf->line, (buf->len + (size_t)length + 1));

    if (temp == NULL) {
        perror("realloc");
        return;
    }

    buf->line = temp;
    memcpy(&buf->line[buf->len], str, (size_t)length);

    buf->len += length;

    buf->line[buf->len] = '\0';
}

void tokenize(const char *str, Token **tokenBuf) {
    char *buf = NULL;
    size_t bufSize = 1;
    static size_t tokenBufSize = 1;

    size_t startPos = 0, endPos = 0;
    int wordStart = 0;
    char c;

    buf = (char *)realloc(buf, bufSize);
    *buf = '\0';

    while (str[startPos] != '\n') {
        while ((c = str[endPos]) != ' ') {
            if (wordStart) {
                char *tempBuf = (char *)realloc(buf, bufSize + 1);
                if (tempBuf == NULL) {
                    free(buf);
                    perror("realloc");
                    return;
                }

                buf = tempBuf;
                buf[bufSize - 1] = c;
                buf[bufSize] = '\0';

                ++bufSize;
            }
            else {
                if (c == '#') {
                    /*
                    * no implementation for macros
                    * return from the function telling no need to compile the macros
                    */
                    free(buf);
                    return;
                }
                // check if the word starts with i.
                if ((c == 'i') && (startPos == endPos)) {
                    char *tempBuf = (char *)realloc(buf, bufSize + 1);
                    if (tempBuf == NULL) {
                        free(buf);
                        perror("realloc");
                        return;
                    }

                    buf = tempBuf;
                    buf[bufSize - 1] = c;
                    buf[bufSize] = '\0';

                    ++bufSize;
                    wordStart = 1;
                }
            }
            endPos++;
        }

        if (!strcmp(buf, "int")) {
            *tokenBuf = (Token *)realloc(*tokenBuf, sizeof(Token) * tokenBufSize);
            if (*tokenBuf == NULL) {
                free(buf);
                perror("realloc");
                return;
            }

            (*tokenBuf)[tokenBufSize - 1].value = NULL;
            (*tokenBuf)[tokenBufSize - 1].value = (char *)realloc((*tokenBuf)[tokenBufSize - 1].value, bufSize);
            if (((*tokenBuf)[tokenBufSize - 1].value) == NULL) {
                free(buf);
                perror("realloc");
                return;
            }

            (*tokenBuf)[tokenBufSize - 1].tokenType = KEYWORD;
            strcpy((*tokenBuf)[tokenBufSize - 1].value, buf);

            ++tokenBufSize;
        }

        memset(buf, '\0', bufSize);
        buf = (char *)realloc(buf, 1);

        wordStart = 0;
        bufSize = 1;
        startPos = endPos + 1;
        endPos = startPos;
    }

    free(buf);
}
