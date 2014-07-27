#ifndef STRING_C
#define STRING_C

#include <string.h>

int strlen(char *str)
{
	char *temp = str;
	int count = 0;

	while (*temp++) {
		count++;
	}

	return count;
}

void reverse(char *str)
{
	int i, j;
 
	for (i = 0, j = strlen(str) - 1; i < j; i++, j--) {
		swap(str + i, str + j);
	}
}

void swap(char *a, char *b)
{
	char temp = *a;
	*a = *b;
	*b = temp;
}

void itoa(int val, char *str)
{
	char *buffer = str;
	char sign = 0;
	char i = 0;
	
	if (val < 0) {
		sign = 1;
		val = -val;
	}

	do {
		*(buffer + i) = val % 10 + '0';
		i++;
	} while ((val /= 10) > 0);

	if (sign) {
		*(buffer + i) = '-';
		i++;
	}

	*(buffer + i) = '\0';

	reverse(buffer);
}

#endif
