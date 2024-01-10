#include "main.h"
#include "_stdlib.h"

/**
 * start_with - checks if command starts with another string
 * @string: the string characters
 * @start: begining string
 * Return: 1 if starts with, 0if not
*/
int start_with(char *string, char *start)
{
	int i;

	for (i = 0; (string[i] != '\0' && start[i] != '\0'); i++)
	{
		if (string[i] != start[i])
			return (0);
	}
	return (start[i] == '\0');
}

/**
 * _getenv - searches the environment list for a variable name.
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
