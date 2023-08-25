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
	char *trimmed_line;

	while (1)
	{
		if (isatty(0))
			write(STDOUT_FILENO, "$ ", 2);
		(void)ac;
		(void)av;
		read = getline(&line, &len, stdin);
		if (read == -1)
			break;
		trimmed_line = trim(line);
		if (strcmp(trimmed_line, "exit") == 0)
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
	full_path = find_path(tokens[0]);
	if (full_path == NULL)
	{
		free_array(tokens, token_count);
		return;
	}
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
 * @command_name: given command
 * Return: full_path which is the command
 */

char *find_path(char *command_name)
{
	char *command_path = "/bin/";
	char *full_path;

	if (access(command_name, X_OK) == 0)
	{
		full_path = strdup(command_name);
		if (full_path == NULL)
		{
			perror("malloc");
			return (NULL);
		}
		return (full_path);
	}

	full_path = malloc(strlen(command_path) + strlen(command_name) + 1);
	if (full_path == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	sprintf(full_path, "%s%s", command_path, command_name);

	if (access(full_path, X_OK) != 0)
	{
		perror("Command not found");
		free(full_path);
		return (NULL);
	}

	return (full_path);
}
