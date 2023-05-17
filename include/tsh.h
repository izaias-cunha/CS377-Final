#ifndef _SIMPLE_SHELL_H
#define _SIMPLE_SHELL_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

class simple_shell {
 public:
  void parse_command(char* cmd, char** cmdTokens);
  void exec_command(char** argv);
  void exec_pipe_command(char** argv);
  void exec_bash(char* argv);
  int contains(char * arr, char c);
  void split_token(char** cmd, char** token1, char** token2);
  bool isBash(char* cmd);
  bool isQuit(char* cmd);
};

#endif