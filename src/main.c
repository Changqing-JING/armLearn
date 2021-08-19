#include <stdlib.h>

void lab_memset(void* dist, int value, size_t size);

int main(){

    char d[100];

    lab_memset(&d, 10, 48);
    
    return 0;
}