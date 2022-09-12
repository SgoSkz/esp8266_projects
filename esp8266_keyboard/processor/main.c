#include <stdio.h>
#include <stdlib.h>

char* before(char* str);
char* after(char* str);

int main(int argc, char** argv) {
    printf("%d\n", argc);
    for(int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
        printf("ret: %s\targv: %s\n", after(argv[i]), argv[i]);
    }
}

/**
 * Saves all characters before comma
 */
char* before(char* str) {
    char *src, *dst;
    int del = 0;
    for(src = str, dst = str; *src; src++) {
        if(*src == ',') del = 1;
        if(del == 1) continue;
        *dst++ = *src;
    }
    *dst='\0';
    return str;
}

/**
 * Saves all the characters after comma
 */
char* after(char* str) {
    char *src, *dst;
    int del = 1;
    for(src = str, dst = str; *src; src++) {
        if(*src == ',') del = -1;
        if(del != 0) {del++; continue;}
        /* if(del == -1) {del++; continue;} */
        *dst++ = *src;
    }
    *dst='\0';
    return str;
}
