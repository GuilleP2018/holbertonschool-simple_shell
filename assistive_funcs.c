#include "simple_shell.h"

/**
 * trim - removes whitespace
 * @str: string given
 */

void trim(char *str)
{
	int start = 0;
	int end = strlen(str) - 1;
	int index = 0;
	int i;

	while (isspace(str[start]))
		start++;

	while (end >= 0 && isspace(str[end]))
		end--;

	for (i = start; i <= end; i++)
		str[index++] = str[i];

	str[index] = '\0';
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
