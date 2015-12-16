#ifndef STDLIB_H
#define STDLIB_H

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

typedef unsigned int size_t;
typedef short int wchar_t;

typedef struct div_t div_t;
struct div_t
{
    int value;
    int remainder;
};

typedef struct ldiv_t ldiv_t;
struct ldiv_t
{
    long int value;
    long int remainder;
};

int abs(int);

long int labs(long int);

void itoa(int val, char* str);

#endif