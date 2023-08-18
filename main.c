#include "simple_shell.h"

/**
 * main - main function of the simple shell
 * @ac: argument count (voided)
 * @av: array for arguments (voided)
 * @env: environment var, gives the environment of command
 *
 * Return: int
 */

int main(int ac, char **av, char **env)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int non_interactive = isatty(STDIN_FILENO) == 0;

	if (non_interactive)
	{
		non_interactive_mode(env);
	}
	else
	{
		while (1)
		{
			(void)ac;
			(void)av;
			printf("($) ");
			read = getline(&line, &len, stdin);
			if (read == -1)
				break;
			if (line[read - 1] == '\n')
				line[read - 1] = '\0';
			if (strcmp(line, "exit") == 0)
				break;
			if (isspace((unsigned char)line[0]))
				continue;
			if (line[0] == '\0' || line[0] == ' ')
				continue;
			exec_command(line, env);
		}
		free(line);
		return (0);
	}
	free(line);
	return (0);
}

/**
 * exec_command - function that executes given command
 * @command: command recieved
 * @env: environment recieved
 */

void exec_command(char *command, char **env)
{
	pid_t child_pid;
	char *token = NULL, **tokens = NULL;
	int token_count = 0;

	get_paths();
	token = strtok(command, " \n");
	if (token == NULL)
		return;
	while (token != NULL)
	{
		tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
		if (tokens == NULL)
		{
			perror("realloc");
			return;
		}
		tokens[token_count] = token;
		token_count++;
		token = strtok(NULL, " \n");
		tokens[token_count] = NULL;
	}
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		free(tokens);
		return;
	}
	if (child_pid == 0)
	{
		execve(tokens[0], tokens, env);
		perror("./hsh");
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(NULL);
	}
	free(tokens);
}

/**
 * get_paths - gets path of the command
 * Return: the path of the command
 */

char **get_paths(void)
{
	char *path_env = NULL;
	char *token = NULL;
	int num_paths = 0;
	char **paths = NULL;

	path_env = getenv("PATH");
	if (path_env == NULL)
	{
		perror("error ");
		return (NULL);
	}
	token = strtok(path_env, ":");
	while (token != NULL)
	{
		paths = realloc(paths, sizeof(char *) * (num_paths + 1));
		if (paths == NULL)
		{
			perror("error ");
			free(paths);
			return (NULL);
		}
		paths[num_paths] = token;
		num_paths++;
		token = strtok(NULL, ":");
	}
	paths = realloc(paths, sizeof(char *) * (num_paths + 1));
	paths[num_paths] = NULL;

	return (paths);
}

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
