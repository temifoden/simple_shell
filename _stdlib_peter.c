#include "main.h"
#include "_stdlib.h"

/**
 * _getenv - searches the environment list for a variable name, returns a pointer to its value.
 * @name: the name of the variable
 * Return: a pointer to the value of the variable, NULL if not found
 */
char *_getenv(const char *name)
{
	int i = 0;
	int j = 0;

	if (environ == NULL)
		return (NULL);
	while (environ[i])
	{
		j = 0;
		while (name[j] && environ[i][j] == name[j])
			j++;
		if (name[j] == '\0' && environ[i][j] == '=')
			return (environ[i] + j + 1);
		i++;
	}
	return (NULL);
}
