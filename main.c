#define _GNU_SOURCE
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <string.h>
// #include <ctype.h>
#include <stdio.h>

typedef enum {
    DATATYPE_INT,
    MAIN,
    OPEN_CURLY_BRACKET,
    CLOSE_CURLY_BRACKET,
    OPEN_SMALL_BRACKET,
    CLOSE_SMALL_BRACKET,
    PRINTF,
    DOUBLE_QUOTES,
    STRING,
    INT_VAL,
    RETURN,
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
void tokenize(const char *str, Token **tokenBuf, size_t *tokenBufSize);

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
    size_t tokenBufSize = 1;
    size_t length;
    ssize_t numOfCharRead;

    while ((numOfCharRead = getline(&line, &length, fp)) != -1) {
        tokenize(line, &tokenBuf, &tokenBufSize);
        appendLine(&charBuf, line, numOfCharRead);
    }

    // error handling so that if no memory is allocated, we would not be freeing null memory.
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

void tokenize(const char *str, Token **tokenBuf, size_t *tokenBufSize) {
    char *buf = NULL;
    size_t bufSize = 1;

    size_t startPos = 0, endPos = 0;
    char c;

    buf = (char *)realloc(buf, bufSize);
    *buf = '\0';

    while (str[startPos] != '\n') {
        while (((c = str[endPos]) != ' ') || ((c = str[endPos]) != '\n')) {
            if (c == '#') {
                /*
                    * no implementation for macros
                    * return from the function telling no need to compile the macros
                    */
                free(buf);
                return;
            }
            else {
                // check if we have () after a function name.
                if ((c == '(') && (str[endPos + 1] != ')')) {
                    // TODO: implement ( paxi ko "" in printf.
                    break;
                }

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
            
            endPos++;
        }

        // printf("%s", buf);

        if ((!strcmp(buf, "int")) || (!strcmp(buf, "main()"))) {
            *tokenBuf = (Token *)realloc(*tokenBuf, sizeof(Token) * (*tokenBufSize));
            if (*tokenBuf == NULL) {
                free(buf);
                perror("realloc");
                return;
            }

            (*tokenBuf)[(*tokenBufSize) - 1].value = NULL;
            (*tokenBuf)[(*tokenBufSize) - 1].value = (char *)realloc((*tokenBuf)[(*tokenBufSize) - 1].value, bufSize);
            if (((*tokenBuf)[(*tokenBufSize) - 1].value) == NULL) {
                free(buf);
                perror("realloc");
                return;
            }

            (*tokenBuf)[(*tokenBufSize) - 1].tokenType = DATATYPE_INT;
            strcpy((*tokenBuf)[(*tokenBufSize) - 1].value, buf);

            ++(*tokenBufSize);
        }

        memset(buf, '\0', bufSize);
        buf = (char *)realloc(buf, 1);

        bufSize = 1;
        startPos = endPos + 1;
        endPos = startPos;
    }

    free(buf);
}

// void appendToken(char *buf, size_t bufSize, Token **tokenBuf, size_t *tokenBufSize) {
//
// }
