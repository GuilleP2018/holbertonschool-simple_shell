#include "simple_shell.h"

/**
 * trim - removes whitespace
 * @str: string given
 * Return: trimmed str
 */

char *trim(char *str)
{
	char *start = str;
	char *end = str + strlen(str) - 1;

	while (isspace((unsigned char)*start))
		start++;

	while (end > start && isspace((unsigned char)*end))
		end--;

	end[1] = '\0';

	return (start);
}

/**
 * free_array - Frees memory allocated for arrays
 * @array: The array to be freed
 * @n: The number of strings in the array
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


