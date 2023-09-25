#include <stdio.h>
#include <stdlib.h>

int main() {
    char* c = "abcdef";
    char* p1 = c;
    char** p2 = &c;

    *p2 += 1;

    printf("%s\n", c);
    printf("%s\n", p1);
    printf("%s\n", *p2);
    printf("%c\n", **p2);

    return 0;
}