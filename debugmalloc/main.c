#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugmalloc.h"

int main(void) {
    char txt[] = "hello";

    char *p = malloc(strlen(txt) * sizeof(char) + 1);
    strcpy(p, txt);
    printf("TXT: %s\n", p);

    free(p);

    return 0;
}
