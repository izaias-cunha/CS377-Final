#include <tsh.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

/*
 *  This function is used to parse the commands into tokens that will be used for execv.
 *
 *  Parameters: char* cmd: the input from the user that will be parsed
 *              char** cmdToken: The pointer to an array that will hold the parsed token
 */
void simple_shell::parse_command(char* cmd, char** cmdTokens) {
    int i = 0; //tracks index of cmd
    int j = 0; //tracks index of cmdToken
    string str = ""; //holds the words while cmd is parsed
    bool con = true; //used to detemine if parsing is complete

    //Itterates through the command and splits the words
    while (con) {

        if (cmd[i] != ' ' && cmd[i] != '\n') {
            str += cmd[i];
        } else {
            if (cmd[i] == '\n') {
                con = false;
            }
            char *cp = new char[str.length() + 1];
            strcpy(cp, str.c_str());
            cmdTokens[j] = cp;
            str = "";
            j++;
        }

        i++;
    }

    cmdTokens[j] = NULL;
}

/*
 *  This functions executes commands that do not require a pipe for execution. This function forks
 *  a child which will execute the command
 * 
 *  Parameters: char** argv: Contains to UNIX caommand that will be executes and its arguments
 */
void simple_shell::exec_command(char** argv) {
    int pid = fork();

    if (pid == 0) {
        int status_code = execvp(argv[0], argv);
        if (status_code == -1) {
            cout << "tsh: execution error \n";
        }
    } else {
        int wait = waitpid(pid, NULL, 0); 
        kill(pid, SIGKILL);
    }
    
}

/*
 *  This functions executes commands that require a pipe for execution (ex. "ls -laF | tail"). This 
 *  function forks two children which will execute each command. This utalizes a pipe to communicate 
 *  between the two command executions
 * 
 *  Parameters: char** argv: Contains to UNIX caommand that will be executes and its arguments
 */
void simple_shell::exec_pipe_command(char** argv) {
    //split argv into two arrays
    char* args1[25];
    char* args2[25];
    split_token(argv, args1, args2);

    //Pipe creation and error checking
    int pip[2];
    int r = pipe(pip);
    if (r != 0) {
        cout << "tsh: Pipe Error";
        exit(0);
    }

    int pid = fork();

    if (pid == 0) { //executes first command
        close(pip[0]);
        dup2(pip[1], STDOUT_FILENO);
        close(pip[1]);
        int status_code1 = execvp(args1[0], args1);

        if (status_code1 == -1) {
            cout << "tsh: execution error \n";
        }
    } else {
        close(pip[1]);
        int wait = waitpid(pid, NULL, 0);
        int pid2 = fork();

        if (pid2 == 0) { //executes second command
            dup2(pip[0], STDIN_FILENO);
            close(pip[0]);
            int status_code2 = execvp(args2[0], args2);

            if (status_code2 == -1) {
                cout << "tsh: execution error \n";
            }
        } else {
            int wait2 = waitpid(pid2, NULL, 0);
            close(pip[0]);
            kill(pid, SIGKILL);
            kill(pid2, SIGKILL);
        }
    }
}

/*
 *  This function is used to execute all bash commands, to incluse executing bash scripts. It forks a child 
 *  and calls execl to use bash with the user command as a parameter 
 *
 *  Parameters: char* argv: command inputed by user
 */
void simple_shell::exec_bash(char* argv) {
    int pid = fork();

    if (pid == 0) {
        int status_code = execl("/bin/bash", "bash", "-c", argv, NULL);

        if (status_code == -1) {
            cout << "tsh: execution error \n";
        }
    } else {
        int wait = waitpid(pid, NULL, 0);
        kill(pid, SIGKILL);
    }
}

/*
 *  This a helper functions iterates through the user input to determine if it contain c and requires a pipe to execute it.
 *  Only called when using UNIX commands as the bash commands do not require a pipe.
 * 
 *  Parameters: char* cmd: user input
 *              char c: the character to look for in input
 * 
 *  Return: 1 - contains c / 0 - does not contain c
 */
int simple_shell::contains(char* cmd, char c) {
  int i = 0; //index of cmd

  while (cmd[i] != '\n') {
    if (cmd[i] == c) return 1;
    i++;
  }
  return 0;
}

/*
 *  This is a helper function that is used when the command requires a pipe and it splits it into two seperate arg arrays
 *  for future execution. It will split cmd on char '|' and token 1 contains the left side and token2 contains the right side.
 * 
 *  Parameters: char** cmd: the tokenized command to be split
 *              char** token1: the resulting left hand side of '|' from cmd
 *              char** token2: the resulting right hand side of '|' from cmd
 */
void simple_shell::split_token(char** cmd, char** token1, char** token2) {
    int i = 0; //index of cmd
    int j = 0; //index for token1 and token2
    int f = 0; //condition to determine when to switch between populating token1 and token2
    char a = '|';

    while(cmd[i] != NULL) {
        if (f) {
            token2[j] = cmd[i];
            j++;
        } else {
            if (*cmd[i] == a) {
                f = 1;
                token1[j] = NULL;
                j = 0;
            } else {
                token1[j] = cmd[i];
                j++;
            }
        }
        i++;
    }

    token2[j] = NULL;
}

/*
 *  This function checks if the user input a bashmode command
 *  
 *  Parameters: char* cmd: User input
 * 
 *  Returns: True - user inputed "bashmode" / False - user did not input "bashmode"
 */
bool simple_shell::isBash(char* cmd) {
    if (strcmp(cmd, "bashmode") == 0) {
        return true;
    }
    return false;
}

/*
 *  This function checks if the user input a quit command
 *  
 *  Parameters: char* cmd: User input
 * 
 *  Returns: True - user inputed "quit" / False - user did not input "quit"
 */
bool simple_shell::isQuit(char* cmd) {
    if (strcmp(cmd, "quit") == 0) {
        return true;
    }
    return false;
}
