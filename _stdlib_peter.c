#include "main.h"
#include "_stdlib.h"

/**
 * _getenv - searches the environment list for a variable name, returns a pointer to its value.
 * @name: the name of the variable
 * Return: a pointer to the value of the variable, NULL if not found
 */
char *_getenv(const char *name)
{
	return (getenv(name));
}
