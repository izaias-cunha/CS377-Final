This final project is an Extension of Simple Shell (P2) to include:
    - piping commands (ex. "ls -laF | tail") 
    - bash commands (ex "echo hello world!")
    - bash script execution

In order to execute any bash commands or bash scipt, you must put the 
shell in Bash Mode. This is done by inputing the command "bashmode".

'bashmode' is used to both enter and exit bashmode. Upon typing the 
command, the shell will either return "Bash Mode: ON" or "Bash Mode: 
OFF". If in bash mode, you can only execute bash commands.

To execute a bash script, place script in root directory of project. 
After executing the "tsh_app" enter bash mode with the command "bashmode" 
and then type "bash filename.sh".

I have included a demo bash script, create_file.sh, that creates a text 
file and writes "Hello! This is my CS377 Final Project." to it.

The execution of commands in the shell use three seperate functions. One 
for single UNIX commands, piping UNIX commands, and bash commands. This 
was done this way, because they are each executed differently. Piping UNIX
commands require two child processes, a pipe, and two sets of argv arrays.
Executing bash commands requires the use of execl and pasing in the command
inputed to /bash for execution.

Presentation link: https://drive.google.com/file/d/1Z9NcfmXre44txdJ53t6Cb7UBs2MgQSGq/view?usp=sharing