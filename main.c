#include "main.h"
#include "_stdlib.h"

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
char **tokenize(char *str)
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

	/* allocat just enough memory to store the tokens (just pointers to them); */
	tokens = malloc(sizeof(char *) * (tc + 1));
	if (!tokens) /* malloc failed, nooooooooo!!! */
		return (NULL);

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
/**
 * main - entry point into this awesome shell
 * @ac: number of command line args
 * @av: the command line args
 * Return: 0 on success, appropriate error no on fail
 */
int main(int ac, __attribute__((__unused__)) char **av)
{
	char *lineptr = NULL; /* getline stores the line from stdin here */
	size_t n = 0; /* size of buffer(lineptr) allocated to store line. getline updates it accordingly */
	ssize_t len; /* no of chars read by getline */

	char **tokens = NULL; /*array of tokens gotten from tokenize. */

	int i; /* variable to use in loops */

	if (ac > 1) /* passing the shell a script (a file)*/
	{
		_puts("file execution not constructed, come back later");
		return (0);
	}

	if (!isatty(STDIN_FILENO)) /* running shell by piping input into it */
	{
		_puts("pipes not constructed, come back later");
		return (0);
	}

	/* running the shell in interactive mode */
	printf("$ ");
	len = getline(&lineptr, &n, stdin);
	while (len != -1) /* getline returns -1 when it reaches eof */
	{
		tokens = tokenize(lineptr);
		if (*tokens == NULL) /* user entered nothing */
		{
			continue;
		}

		/* printf out the tokens to test our awesome function */
		for (i = 0; tokens[i]; i++)
		{
			printf("%d: %s \n", i, tokens[i]);
		}

		printf("$ "); /* prompt the user again and again */
		len = getline(&lineptr, &n, stdin);
	}

	return (0);
}
