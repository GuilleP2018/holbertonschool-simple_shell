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

void tokenize(char *command, char **env);
void non_interactive_mode(char **env);
char **get_paths(void);
void exec_command(char **tokens, char **env, char **paths);

#endif
