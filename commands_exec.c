#include "smallsh_main.h"
#include "commands_exec.h"

/////////////////////////////////////////////////////////////
//Called for all commands that are not the built in comamnds
///Will fork process and run exec command, child process deals
// with the input output changes
void executeCommand(Command *cmd) {
    int exitStatus;
    int termSignal;
    int status;
    pid_t spawnpid;
    
    spawnpid = fork();
    switch(spawnpid)
    {
        //FAILURE
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
            break;


        case 0:
            //child process
            if (cmd->background == 1 && isForegroundOnly == 0){
                printf("Background proccess started %d\n", getpid());
                fflush(stdout);
            };

            //changing input
            if (strlen(cmd->input_file) > 0) {
                int inputFile = open(cmd->input_file, O_RDONLY);
                if (inputFile == -1) {
                    perror("open");
                    exit(1);
                }
                if (dup2(inputFile, STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(1);
                }
                close(inputFile);
            }

            //output redirection
            if (strlen(cmd->output_file) > 0) {
                int outputFile = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (outputFile == -1) {
                    perror("open");
                    exit(1);
                }
                if (dup2(outputFile, STDOUT_FILENO) == -1) {
                    perror("dup2");
                    exit(1);
                }
                close(outputFile);
            }

            //runs the exec command after setting up prereqs
            if (execvp(cmd->command, cmd->args) == -1) {
                perror("error");
                exit(EXIT_FAILURE);
            };
            break;


        default:
            //parent process
            //either waits for child process if its foreground, or doesnt wait if the command is background
            if (cmd->background == 1 && isForegroundOnly == 0){
                usleep(100);
            }
            else {
                waitpid(spawnpid, &status, 0);
                ///exiting statuses
                if (WIFEXITED(status) != 0) {
                    exitStatus = WEXITSTATUS(status);
                }
                else if (WIFSIGNALED(status) != 0) {
                    signalCalled = 1;
                    printf("The process was terminated by a signal\n");
                    termSignal = WTERMSIG(status);
                    printf("The term signal was %d\n", termSignal);
                }
            };
            break;
    }
    //executed by all
}





/////////////////////////////////////////////
///////For built in command, cd, status, exit
void executeBuiltIn(Command *cmd) {
    if (strcmp(cmd->command, "cd") == 0) {
        //change directory
        if (cmd->args[1] == NULL) {
            //is no args then goto home
            chdir(getenv("HOME"));
        } else {
            if (chdir(cmd->args[1]) != 0) {
                perror("cd");
            }
        }
    } else if (strcmp(cmd->command, "exit") == 0) {
        exit(0); }
    else if (strcmp(cmd->command, "status") == 0) {
        //print the exit status of last process
        if (WIFEXITED(status) != 0) {
            printf("Exit status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status) != 0) {
            printf("Terminated by signal: %d\n", WTERMSIG(status));
        }
    }
}
