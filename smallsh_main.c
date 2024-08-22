#include "smallsh_main.h"
#include "parsing_inputs.h"
#include "commands_exec.h"

int isForegroundOnly = 0;
int signalCalled = 0;
int parentsleep = 0;

/////CTRL Z SIGSTP
//Toggle switch that will change the global variable whenever its called
void handleSIGTSTP(int signo) {
    signalCalled = 1;
    if (isForegroundOnly == 0) {
        char* message = "\nENTERING FOREGROUND ONLY MODE.\n";
        write(STDOUT_FILENO, message, 32);
        isForegroundOnly = 1;
    }
    else {
        char* message = "\nEXITING FOREGROUND ONLY MODE.\n";
        write(STDOUT_FILENO, message, 31);
        isForegroundOnly = 0;
    }
};

//////SIG CHILD
//For returned children, if their background it will print that it has finished
void handleSIGCHLD(int signo) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status) != 0) {
            printf("\nBackground process %d finished. exit status %d\n", pid, status);
            waitpid(0, &status, 0);
            signalCalled = 1;
        } else if (WIFSIGNALED(status) != 0) {
            printf("Background process %d terminated by signal %d\n", pid, WTERMSIG(status));
            waitpid(0, &status, 0);
            signalCalled = 1;
        }
    }
}

int main() {
    
    /////////////////////////////////
    //SIG STP signal controling//////
    struct sigaction SIGTSTP_action = {0};
    SIGTSTP_action.sa_handler = handleSIGTSTP;
    sigfillset(&SIGTSTP_action.sa_mask);
    SIGTSTP_action.sa_flags = 0;
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);


    ////////////////////////////////////
    ////SIGCHLD HANDELING//////////
    struct sigaction SIGCHLD_action = {0};
    SIGCHLD_action.sa_handler = handleSIGCHLD;
    sigfillset(&SIGCHLD_action.sa_mask);
    SIGCHLD_action.sa_flags = 0;
    sigaction(SIGCHLD, &SIGCHLD_action, NULL);


    while (1) {
        //GETTING INPUT
         if (signalCalled == 1){
            signalCalled = 0;
            continue;
        };
        printf(": ");
        fflush(stdout);

        char input[MAX_INPUT_SIZE];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        //This is for when signal interupts loop, it will skip the iteration
        //Becuase signals would cause the last called cmd to run twice
        if (signalCalled == 1){
            strcpy(input, "");
            signalCalled = 0;
            continue;
        };
        Command cmd;
        cmd.background = 0;
        parseInput(input, &cmd);

        //after parseing, will send to either built in function, or normal exucutecommand
        if (strcmp(cmd.command, "#") != 0) {
            if (strcmp(cmd.command, "exit") == 0 || (strcmp(cmd.command, "status") == 0) || (strcmp(cmd.command, "cd") == 0)) {
                executeBuiltIn(&cmd);
            }
            else {
                executeCommand(&cmd);
            }
        }
        /////////FIX ME//////////
        //////////add built in commands  ********* do not need background capabilites
    }
};