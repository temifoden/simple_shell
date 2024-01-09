#include "main.h"

/**
 * _strlen - returns the length of a string
 * @s: the string
 * Return: the length of the string
 */
size_t _strlen(const char *s)
{
	return (strlen(s));
}

/**
 * _puts - write a string to stdout, followed by newline
 * @str: the string
 * Return: void
 */
void _puts(const char *str)
{
	write(STDOUT_FILENO, str, _strlen(str));
	write(STDOUT_FILENO, "\n", 1);
}

char *_strncat(char *dest, const char *src, size_t n)
{
	return (strncat(dest , src , n));
}

char *_strcpy(char *dest, const char *src)
{
	return (strcpy(dest, src));
}

char *_getenv(const char *name)
{
	return (getenv(name));
}
