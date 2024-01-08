#include "main.h"
#include "_stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct shell_properties/* to be moved to main.h */
{
	char *prog_name;
	int isatty;
} shell_properties;

/**
 * isdelim - determines whether a char is a delimiter character or not
 * @c: the char
 * Return: 1 if the char is a delim, 0 if not
 */
int isdelim(char c)
{
	/*  if a char is any of these, then it is a delimiter */
	return (c == ' ' || c ==  '\n' || c == '\0' || c == '\t');
}
/**
 * tokenize - return an array of tokens from a string
 * @str: the string
 * Return: a pointer to an array of strings(the tokens), NULL on fail
 */
char **tokenize(shell_properties sh, char *str)
{
	char **tokens = NULL;
	int tc = 0; /* token count */
	int i; /* variable for loops */

	for (i = 0; str[i]; i++) /* figure out the number of tokens before hand */
	{
		if (!isdelim(str[i]) && isdelim(str[i + 1]))
		{ /* check if the current char is at the end of a token  */
			tc++; /* increment token count */
		}
	}
	if (tc == 0)
		return (NULL);

	/* allocate just enough memory to store the tokens (just pointers to them); */
	tokens = malloc(sizeof(char *) * (tc + 1));
	if (!tokens) /* malloc failed, nooooooooo!!! */
	{
		perror(sh.prog_name);
		return (NULL);
	}

	i = 0; /* make sure to init variable before using in the loop */
	tokens[i] = strtok(str, " \n\t"); /* get the first token */
	while (tokens[i++]) /* iteratively get the rest of the tokens */
	{
		tokens[i] = strtok(NULL, " \n\t");
	}
	/*
	 * we don't have to manually add a null pointer at the end of the tokens array
	 * because strtok returns NULL for the final token(no more tokens)
	 */

	return (tokens);
}

int execute(shell_properties sh, char *command, char **args)
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		execve(command, args, environ);
		perror(sh.prog_name);
		return (1);
	}
	else if (pid == -1)
	{
		perror(sh.prog_name);
		return (-1);
	}
	else
	{
		if (wait(NULL) == -1)
		{
			perror(sh.prog_name);
			return (-1);
		}
	}
	return (0);
}

/**
 * start_with - checks if command starts with another string
 * @string: the string characters
 * @start: begining string
*/

int start_with(char *string, char *start)
{
	int i;
	for (i = 0; (string[i] != '\0' && start[i] != '\0'); i++)
	{
		if(string[i] != start[i])
			return(0);
	}
	return(start[i]== '\0');
}


/**
 * findCommand - entry
 * @sh: our struct
 * @command: command string
*/

char *findCommand(shell_properties sh, char *command)
{
	struct stat st;
	char * path_copy;
	if (start_with(command, "/") || start_with(command, "../") || start_with(command, "./"))
	{
		if(stat(command, &st) == -1)
		{
			perror(sh.prog_name);
			return(NULL);
		}
		return(command);
	}
	/*please work on this*/
	if (!getenv("SHELL") );
	{
		printf("The path could not be found\n");
		return (NULL);
	}
	path_copy = malloc(sizeof (char) * (_strlen(getenv("SHELL")) + 1));

	if (path_copy == NULL)
	{
		perror(sh.prog_name);
		return (NULL);
	}

	strcpy(path_copy, getenv("SHELL"));
	printf("%s \n",path_copy);
	return(command);
}

/**
 * main - entry point into this awesome shell
 * @ac: number of command line args
 * @av: the command line args
 * Return: 0 on success, appropriate error no on fail
 */
int main(int ac,  char **av)
{
	char *lineptr = NULL; /* getline stores the line from stdin here */
	size_t n = 0; /* size of buffer(lineptr). getline updates it accordingly */
	ssize_t len; /* no of chars read by getline */
	char **tokens = NULL; /*array of tokens gotten from tokenize. */
	char *command = NULL;
	shell_properties sh;

	sh.prog_name = av[0];
	sh.isatty = isatty(STDIN_FILENO);
	if (ac > 1) /* passing the shell a script (a file)*/
	{
		_puts("file execution not constructed, come back later");
		return (0);
	}
	if (!sh.isatty) /* running shell by piping input into it */
	{
		_puts("pipes not constructed, come back later");
		return (0);
	}

	/* running the shell in interactive mode */
	printf("$ ");
	len = getline(&lineptr, &n, stdin);
	while (len != -1) /* getline returns -1 when it reaches eof */
	{
		tokens = tokenize(sh, lineptr);
		if (tokens == NULL)
			goto reprompt;
		if (strcmp("exit", tokens[0]) == 0)
			break;
		command = findCommand(sh, tokens[0]);
		if (command == NULL)
			goto reprompt;
		if (execute(sh, command, tokens) == 1)
			break;

reprompt:
		free(tokens);
		tokens = NULL; /* precaution */
		printf("$ "); /* prompt the user again and again */
		len = getline(&lineptr, &n, stdin);
	}
	if (len == -1)
		_puts("");
	free(tokens);
	free(lineptr);
	return (0);
}
