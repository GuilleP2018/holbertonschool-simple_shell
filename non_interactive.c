#include "simple_shell.h"

/**
 * non_interactive_mode - function to handle non interactive mode
 * @env: environment var
 */

void non_interactive_mode(char **env)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, stdin)) != -1)
	{
		if (read > 0 && line[read - 1] == '\n')
			line[read - 1] = '\0';
		if (strcmp(line, "exit") == 0)
			break;
		if (line[0] == '\0' || line[0] == ' ')
			continue;
		exec_command(line, env);
	}
	free(line);
}

