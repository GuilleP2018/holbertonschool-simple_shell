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
	char *full_path;

	token = strtok(command, " \n");
	if (token == NULL)
		return;
	tokens = malloc(sizeof(char *));
	if (tokens == NULL)
	{
		perror("malloc");
		return;
	}
	while (token != NULL)
	{
		tokens = realloc(tokens, sizeof(char *) * (token_count + 1));
		if (tokens == NULL)
		{
			perror("realloc");
			free_array(tokens, token_count);
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
	if (access(tokens[0], X_OK) == -1)
		full_path = find_path(tokens[0]);
	free(tokens[0]);
	tokens[0] = full_path;
	child_exec(tokens, env);
	free_array(tokens, token_count);
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

	if (tokens == NULL)
	{
		perror("error ");
		return;
	}
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(tokens[0], tokens, env) == -1)
		{
		perror("error ");
		exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(child_pid, &status, 0);
	}
}

/**
 * find_path - finds and concats path to command
 * @command: given command
 * Return: full_path which is the command
 */

char *find_path(const char *command)
{
	char *path = getenv("PATH");
	char *path_copy = strdup(path);
	char *dir, *full_path = NULL;

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		full_path = malloc(strlen(dir) + strlen(command) + 2);
		if (full_path == NULL)
		{
			perror("malloc");
			free(path_copy);
			free(dir);
			free(full_path);
			return (NULL);
		}
		strcpy(full_path, dir);
		strcat(full_path, "/");
		strcat(full_path, command);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	free(full_path);
	free(dir);
	free(path);
	return (NULL);
}
