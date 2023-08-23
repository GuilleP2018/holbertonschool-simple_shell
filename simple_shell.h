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


void non_interactive_mode(char **env);
char *full_path_process(char *command, char **paths);
void child_exec(char **tokens, char **env, char *full_path);
void exec_command(char *command, char **env);
char **get_path(void);

#endif
