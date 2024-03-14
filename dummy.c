#include <stdio.h>

int main(){
    int x = 19;
    int y = 43;
    int z = x + y;
    printf("%d\n", z);

    unsigned long value;
    asm volatile("csrr %0, time" : "=r" (value));

}