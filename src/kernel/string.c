#ifndef STRING_C
#define STRING_C

#include <stddef.h>
#include <string.h>

size_t strlen(char* str)
{
    if (str == NULL) {
	return 0;
    }
    
    int count = 0;

    while (*str++) {
	count++;
    }

    return count;
}

void reverse(char* str)
{
    if (str == NULL) {
	return;
    }
    
    int i;
    int j;

    for (i = 0, j = strlen(str) - 1; i < j; i++, j--) {
	swap(str + i, str + j);
    }
}

void swap(char* a, char* b)
{
    if (a == NULL || b == NULL) {
	return;
    }
    
    char temp = *a;
    *a = *b;
    *b = temp;
}

#endif
