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
}

/**
 * child_exec - Create a child process and execute the command
 * @tokens: The tokenized command and arguments
 * @full_path: path concat
 * @env: The environment variables
 */
void child_exec(char **tokens, char **env)
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
		execve(tokens[0], tokens, env);
		perror("error ");
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(NULL);
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
		paths = realloc(paths, sizeof(char *) * (num_paths + 1));
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

/**
 * free_token_array - Frees memory allocated for token array and token strings
 * @tokens: The token array to be freed
 * @token_count: The number of tokens in the array
 */
void free_array(char **array, int n)
{
	int i;

	for (i = 0; i < n; i++)
	{
		free(array[i]);
	}
	free(array);
}

