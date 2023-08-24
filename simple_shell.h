#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>



void child_exec(char **tokens, char **env);
void exec_command(char *command, char **env);
char **get_path(void);
void free_array(char **array, int n);
void trim(char *str);
char *find_path(const char *command);


#endif
