#include "main.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>

/**
 * _strlen - returns the length of a string
 * @s: pointer to the input string
 * Return: the length of the string
 */
size_t _strlen(const char *s)
{
	size_t length = 0;

	while (*s != '\0')
	{
		length++;
		s++;
	}
	return (length);
}

/**
 * _puts - write a string to stdout
 * @str: the string
 * Return: void
 */
void _puts(const char *str)
{
	write(STDOUT_FILENO, str, _strlen(str));
}

/**
 * _puts_err - write a string to stderr
 * @str: the string
 * Return: void
 */
void _puts_err(const char *str)
{
	write(STDERR_FILENO, str, _strlen(str));
}

/**
 * _strncat - concatenates two strings
 * @dest: the destination string
 * @src: the source string
 * @n: max number of bytes to be concated
 * Return: a pointer to dest;
*/
char *_strncat(char *dest, const char *src, size_t n)
{
	char *original_dest = dest;
	size_t i;

	i = 0;
	while (*dest != '\0')
	{
		dest++;
	}
	while (*src != '\0')
	{
		if (i < n)
		{
			*dest = *src;
			dest++;
			src++;
			i++;
		}
		else
		{
			break;
		}
	}
	*dest = '\0';
	return (original_dest);
}

/**
 * _strcpy - copies a string into dest
 * @dest: the destination string
 * @src: the source string
 * Return: a pointer to dest;
*/
char *_strcpy(char *dest, const char *src)
{
	size_t i = 0;

	for (i = 0; src[i]; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';
	return (dest);
}

/**
 * _strcmp - compares two strings
 * @s1: the first string
 * @s2: the second string
 *
 * Return: An integer
*/
int _strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
