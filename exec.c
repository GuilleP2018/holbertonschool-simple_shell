#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * main - execve example
 *
 * Return: Always 0.
 */
int main(void)
{
	const char *delim = " ";
	char *str;
	char *token = NULL; /*{"/bin/ls", "-l", "/tmp/", NULL};*/
	char **tokens = NULL;
	int i, j;

	
	i = getline(&str, 0, STDIN_FILENO);
	if (i == -1)
		return (-1);

	token = strtok(str, delim);

	j = 0;

	while (token != NULL)
	{
		tokens[j] = token;
		j++;
		token = NULL;
		token = strtok(NULL, delim);
	}

	printf("Before execve\n");
	if (execve(tokens[0], tokens, NULL) == -1)
	{
		perror("Error:");
	}
	printf("After execve\n");
	return (0);
}

/**
 * hacer un strtok al path con delim de : 
 * buscar el path, usamos el prototipo con env, 
 * hay que coger el path del terminal usando
 *
 */

/**int main(int ac, char **av, char **env)
{

}

void get_path(char **env)
{


}
*/

