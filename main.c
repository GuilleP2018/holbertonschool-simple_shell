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
	}
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
	char **paths = get_path();

	if (paths == NULL)
		return;

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

	full_path = full_path_process(tokens[0], paths);
	if (full_path != NULL)
	{
		child_exec(tokens, env, full_path);
		free(full_path);
	}
	else
	{
		printf("Command not found: %s\n", tokens[0]);
	}

	free(tokens);
}

/**
 * child_exec - Create a child process and execute the command
 * @tokens: The tokenized command and arguments
 * @full_path: path concat
 * @env: The environment variables
 */
void child_exec(char **tokens, char **env, char *full_path)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		return;
	}
	if (child_pid == 0)
	{
		execve(full_path, tokens, env);
		perror("error ");
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(NULL);
	}
}

/**
 * full_path_process - Constructs full path for command using available paths
 * @command: The command for which to construct the full path
 * @paths: An array of paths to search for the command
 * Return: A dynamically allocated string containing the full path,
 *         or NULL if the command is not found in any of the paths
 */
char *full_path_process(char *command, char **paths)
{
	int i = 0;
	char *full_path = NULL;

	while (paths[i] != NULL)
	{
		full_path = malloc(strlen(paths[i]) + strlen(command) + 2);
		if (full_path == NULL)
		{
			perror("malloc");
			return (NULL);
		}
		if (access(command, X_OK) == -1)
		{
			strcpy(full_path, paths[i]);
			strcat(full_path, "/");
			strcat(full_path, command);
			if (access(full_path, X_OK) == 0)
				return (full_path);
			free(full_path);
			i++;
			break;
		}
		else
			return (command);
	}
	return (NULL);
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

