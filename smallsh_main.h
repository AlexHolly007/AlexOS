#ifndef SMALLSH_MAIN_H
#define SMALLSH_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>


#define MAX_INPUT_SIZE 2048
#define MAX_ARGS 512
#define MAX_ARGS_SIZE 20
// Struct to hold command information
typedef struct {
    char command[MAX_INPUT_SIZE];
    char *args[MAX_ARGS_SIZE];
    char input_file[MAX_INPUT_SIZE];
    char output_file[MAX_INPUT_SIZE];
    int background;
} Command;
int isForegroundOnly;
int signalCalled;
int parentsleep;

void handleSIGTSTP(int signo);
void handleSIGINT(int signo);
void handleSIGCHLD(int signo);
int main();


#endif