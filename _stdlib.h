#ifndef __STDLIB__
#define __STDLIB__

#include <unistd.h>
#include <string.h>

size_t _strlen(const char *s);
void _puts(const char *str);
char *_strncat(char *dest, const char *src, size_t n);
char *_strcpy(char *dest, const char *src);
char *_getenv(const char *name);

#endif /* __STDLIB__*/ 
