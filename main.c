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

	while (1)
	{
		if (isatty(0))
			write(STDOUT_FILENO, "$ ", 2);
		(void)ac;
		(void)av;
		read = getline(&line, &len, stdin);
		if (read == -1)
			break;
		trim(line);
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

/**
 * exec_command - Execute a command using fork and execve
 * @command: The command to execute
 * @env: The environment variables
 */
void exec_command(char *command, char **env)
{
	char *token = NULL;
	char **tokens = NULL;
	int token_count = 0;

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
		tokens[token_count] = strdup(token);
		token_count++;
		token = strtok(NULL, " \n");
	}
	tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
	if (tokens == NULL)
	{
		perror("realloc");
		return;
	}
	tokens[token_count] = NULL;

	child_exec(tokens, env);
	free_array(tokens, token_count);
	free(token);
}

/**
 * child_exec - Create a child process and execute the command
 * @tokens: The tokenized command and arguments
 * @env: The environment variables
 */
void child_exec(char **tokens, char **env)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		return;
	}
	if (child_pid == 0)
	{
		execve(tokens[0], tokens, env);
		perror("error ");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(child_pid, &status, 0);
	}
}

/**
 * get_path - Extracts paths from the PATH environment variable
 * Return: A pointer to an array of strings containing the paths
 *         NULL on failure or if PATH is not found
 */

char **get_path(void)
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
		paths = malloc(strlen(token) + 22);
		if (paths == NULL)
		{
			perror("error ");
			free_array(paths, num_paths);
			return (NULL);
		}
		paths[num_paths] = strdup(token);
		num_paths++;
		token = strtok(NULL, ":");
	}
	paths = realloc(paths, sizeof(char *) * (num_paths + 1));
	if (paths == NULL)
	{
		perror("error ");
		free_array(paths, num_paths);
		return (NULL);
	}
	paths[num_paths] = NULL;
	free(path_env);
	return (paths);
}

