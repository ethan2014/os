#ifndef STDLIB_C
#define STDLIB_C

#include <stdlib.h>
#include <string.h>

int abs(int val)
{
    if (val < 0) {
        return -val;
    }
    
    return val;
}

long int labs(long int val)
{
    if (val < 0) {
        return -val;
    }
    
    return val;
}

void itoa(int val, char* str)
{
    if (str == NULL) {
        return;
    }
    
    char* buffer = str;
    char sign = 0;
    int i = 0;
    
    if (val < 0) {
        sign = 1;
        val = -val;
    }
    
    do {
        buffer[i] = val % 10 + '0';
        i++;
    } while ((val /= 10) > 0);
    
    if (sign) {
        buffer[i] = '-';
        i++;
    }
    
    buffer[i] = '\0';
    
    reverse(buffer);
}

#endif

