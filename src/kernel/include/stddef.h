#ifndef STDDEF_H
#define STDDEF_H

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef short int wchar_t;

typedef unsigned int size_t;
typedef int ssize_t;

typedef int ptrdiff_t;

#endif
