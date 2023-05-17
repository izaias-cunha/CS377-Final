#include <tsh.h>

using namespace std;

int main() {
  char cmd[81]; //holds user input
  char *cmdTokens[25]; //holds tokenized user input
  int i = 0; //used to indicate if a pipe is needed for command
  int bashM = 0; //indicate whether user will be giving bash commands
  simple_shell *shell = new simple_shell(); //shell

  cout << "tsh> ";

  while (fgets(cmd, sizeof(cmd), stdin)) {
    if (cmd[0] != '\n') {
      
      i = shell->contains(cmd, '|');
      shell->parse_command(cmd, cmdTokens);

      //checks if user will be inputing bash commands when the input is "bashmode"
      if (shell->isBash(*cmdTokens)) {
        bashM = (bashM == 1) ? 0 : 1;
        if (bashM) {
          cout << "tsh: Bash Mode: ON \n";
        } else {
          cout << "tsh: Bash Mode: OFF \n";
        }
        cout << "tsh> ";
        continue;
      }
      
      if (shell->isQuit(*cmdTokens)) { //quit
        exit(0); 
      } else if (bashM) { //execute bash commands
        shell->exec_bash(cmd);
      } else {
        if (i) { //executes pipe commands
          shell->exec_pipe_command(cmdTokens);
        } else {
          shell->exec_command(cmdTokens); //normal UNIX command execution
        }
      }
    }
    cout << "tsh> ";
  }
  cout << endl;
  exit(0);
}
