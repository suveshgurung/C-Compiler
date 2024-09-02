#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Invalid usage\r\n");
        printf("USAGE : ./main test.c\r\n");

        exit(EXIT_FAILURE);
    }

    printf("File read successfully\r\n");

    return 0;
}
