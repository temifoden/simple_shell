#ifndef __SHELL__
#define __SHELL__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "_stdlib.h"

/**
 * struct shell_properties - holds info about the shell
 * @prog_name: the shell's av[0]
 * @isatty: whether the shell stdin is coming from a terminal
 */
typedef struct shell_properties
{
	char *prog_name;
	int isatty;
} shell_properties;

extern char **environ;

#endif /* !__SHELL__ */

