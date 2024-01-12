#include "main.h"
#include "_stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
 * @sh: shell properties, for perror
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

/**
 * execute - executes a program(fork and execve)
 * @command: the command
 * @args: the command line args into the program
 * @sh: shell properties, for perror
 * Return: 0 if success, -1 if error, 1 if error doing execve
*/
int execute(shell_properties *sh, char *command, char **args)
{
	pid_t pid;
	int wstatus;

	pid = fork();
	if (pid == 0)
	{
		execve(command, args, environ);
		perror(sh->prog_name);
		return (1);
	}
	else if (pid == -1)
	{
		perror(sh->prog_name);
		return (-1);
	}
	else
	{
		if (wait(&wstatus) == -1)
		{
			perror(sh->prog_name);
			return (-1);
		}
		if (WIFEXITED(wstatus))
		{
			sh->exit_status = WEXITSTATUS(wstatus);
		} /*handle signals too? */
	}
	return (0);
}


/**
 * findCommand - entry
 * @sh: shell properties, for perror
 * @command: command string
 * Return: full path to the command, NULL if not found
*/
char *findCommand(shell_properties *sh, char *command)
{
	struct stat st;
	char *path_copy, *dir_path, *full_path;

	if (start_with(command, "/")
			|| start_with(command, "../")
			|| start_with(command, "./"))
	{
		if (stat(command, &st) == -1)
		{
			perror(sh->prog_name);
			return (NULL);
		}
		return (command);
	}
	/*please work on this*/
	if (!_getenv("PATH"))
	{
		_puts_err(sh->prog_name);
		_puts_err(": 1: ");
		_puts_err(command);
		_puts_err(": not found\n");
		sh->exit_status = 127;
		return (NULL);
	}
	path_copy = malloc(sizeof(char) * (_strlen(_getenv("PATH")) + 1));
	if (path_copy == NULL)
	{
		perror(sh->prog_name);
		return (NULL);
	}

	_strcpy(path_copy, _getenv("PATH"));

	dir_path = strtok(path_copy, ":");
	while (dir_path != NULL)
	{
		full_path = malloc(sizeof(char)
				* (_strlen(dir_path) + _strlen(command) + 2));
		if (!full_path)
		{
			perror(sh->prog_name);
			free(path_copy);
			return (NULL);
		}
		full_path[0] = '\0';
		_strncat(full_path, dir_path, _strlen(dir_path));
		_strncat(full_path, "/", 1);
		_strncat(full_path, command, _strlen(command));
		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		full_path = NULL;
		dir_path = strtok(NULL, ":");
	}
	_puts_err(sh->prog_name);
	_puts_err(": 1: ");
	_puts_err(command);
	_puts_err(": not found\n");
	free(path_copy);
	free(full_path);
	sh->exit_status = 127;
	return (NULL);
}

int is_num(char *str)
{
	int i;
	for (i = 0; str[i]; i++)
	{
		if (i == 0)
		{
			if ((str[i] < '0' || str[i] > '9') && str[i] != '+' && str[i] != '-')
				return 0;
		}
		else
		{
			if ((str[i] < '0' || str[i] > '9'))
				return 0;
		}
	}
	if ((str[0] == '+' || str[0] == '-') && i == 1)
		return (0);
	return (1);
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
	char *old_pwd = NULL;
	char *pwd = NULL;

	int i;

	sh.prog_name = av[0];
	sh.isatty = isatty(STDIN_FILENO);
	sh.exit_status = 0;
	if (ac > 1) /* passing the shell a script (a file)*/
	{
		_puts("file execution not constructed, come back later\n");
		return (0);
	}

	/* running the shell in interactive mode */
	if (sh.isatty)
	{
		_puts(getenv("PWD"));
		_puts("$ ");
	}
	len = getline(&lineptr, &n, stdin);
	while (len != -1) /* getline returns -1 when it reaches eof */
	{
		tokens = tokenize(sh, lineptr);
		if (tokens == NULL)
			goto reprompt;
		if (_strcmp("exit", tokens[0]) == 0)
		{
			if (tokens[1])
			{
				if (!is_num(tokens[1]))
				{
					_puts_err(sh.prog_name);
					_puts_err(": 1: ");
					_puts_err("exit: ");
					_puts_err("Illegal number: ");
					_puts_err(tokens[1]);
					_puts_err("\n");
					sh.exit_status = 2;
					goto reprompt;
				}
				if (atoi(tokens[1]) < 0)
				{
					_puts_err(sh.prog_name);
					_puts_err(": 1: ");
					_puts_err("exit: ");
					_puts_err("Illegal number: ");
					_puts_err(tokens[1]);
					_puts_err("\n");
					sh.exit_status = 2;
					goto reprompt;
				}
				sh.exit_status = atoi(tokens[1]);
			}
			break;
		}
		if (_strcmp("env", tokens[0]) == 0)
		{
			for (i = 0; environ[i]; i++)
			{
				_puts(environ[i]);
				_puts("\n");
			}
			sh.exit_status = 0;
			goto reprompt;
		}
		if (_strcmp("cd", tokens[0]) == 0)
		{
			old_pwd = getcwd(NULL, 0);
			if (!old_pwd)
				perror("getcwd"); /*set exit_status?*/
			else if (!tokens[1])
			{
				if (chdir(getenv("HOME")) == 0)
				{
					pwd = getcwd(NULL, 0);
					setenv("OLDPWD", old_pwd, 1);
					setenv("PWD", pwd, 1);
					free(pwd);
					pwd = NULL;
				}
			}
			else if (_strcmp("-", tokens[1]) == 0)
			{
				if (chdir(getenv("OLDPWD")) == 0)
				{
					pwd = getcwd(NULL, 0);
					setenv("PWD", pwd, 1);
					setenv("OLDPWD", old_pwd, 1);
					free(pwd);
					pwd = NULL;
				}
				_puts(getenv("PWD"));
				_puts("\n");
			}
			else
			{
				if (chdir(tokens[1]) == 0)
				{
					pwd = getcwd(NULL, 0);
					setenv("OLDPWD", old_pwd, 1);
					setenv("PWD", pwd, 1);
					free(pwd);
					pwd = NULL;
				}
				else
				{

					/* /hsh: 1: cd: can't cd to /hbtn */
					_puts_err(sh.prog_name);
					_puts_err(": 1: ");
					_puts_err("cd: ");
					_puts_err("can't cd to ");
					_puts_err(tokens[1]);
					_puts_err("\n");
				}


			}
			free(old_pwd);
			old_pwd = NULL;
			goto reprompt;
		}
		command = findCommand(&sh, tokens[0]);
		if (command == NULL)
			goto reprompt;
		if (execute(&sh, command, tokens) == 1)
			break;

reprompt:
		if (tokens && command != tokens[0])
			free(command);
		command = NULL;
		free(tokens);
		tokens = NULL; /* precaution */
		if (sh.isatty)
		{
			_puts(getenv("PWD"));
			_puts("$ ");
		}
		len = getline(&lineptr, &n, stdin);
	}
	if (len == -1 && sh.isatty)
		_puts("\n");

	if (tokens && command != tokens[0])
		free(command);
	free(tokens);
	free(lineptr);
	return (sh.exit_status);
}
