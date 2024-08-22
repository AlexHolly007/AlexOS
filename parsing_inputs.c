#include "smallsh_main.h"
#include "parsing_inputs.h"

/////////////////////////////////////////////
////Parse input takes the raw input from the command line
////and tries to create a cmd struct with the information.
////Deals with in./out file operations and & for background information
void parseInput(char *input, Command *cmd) {
    char* token_exp;
    while ((token_exp = strstr(input, "$$")) != NULL) {
        char pidString[20]; //create string that will replace
        sprintf(pidString, "%d", getpid());

        //create a new string with the substitution
        char newInput[MAX_INPUT_SIZE];
        strncpy(newInput, input, token_exp - input);
        newInput[token_exp - input] = '\0';
        strcat(newInput, pidString);
        strcat(newInput, token_exp + 2);

        //update it
        strcpy(input, newInput);
    }


    //tokenize the input to get arguments /i/o &
    char *token = strtok(input, " ");
    if (token == NULL || token[0] == '#' || token[0] == '\0') {
        //FAILURE
        strcpy(cmd->command, "#");
        return;
    }

    strcpy(cmd->command, token);
    cmd->args[0] = cmd->command;
    cmd->input_file[0] = '\0';
    cmd->output_file[0] = '\0';
    cmd->background = 0;

    // Parse arguments
    int i = 1;
    while ((token = strtok(NULL, " ")) != NULL && i < MAX_ARGS_SIZE - 1) {
        if (strcmp(token, "&") == 0) {
            cmd->background = 1;
            break;
        } else if (strcmp(token, "<") == 0) {
            //input file found
            token = strtok(NULL, " ");
            if (token != NULL) {
                strcpy(cmd->input_file, token);
            }
        } else if (strcmp(token, ">") == 0) {
            //output file found
            token = strtok(NULL, " ");
            if (token != NULL) {
                strcpy(cmd->output_file, token);
            }
        } else {
            //noraml argument
            cmd->args[i] = strdup(token);
            i++;
        }
    }

    //checking for background processes with no input/output described
    if (cmd->background == 1 && strlen(cmd->input_file) <= 0){
        strcpy(cmd->input_file, "/dev/null");
    };
    if (cmd->background == 1 && strlen(cmd->output_file) <= 0){
        strcpy(cmd->output_file, "/dev/null");
    }
    cmd->args[i] = NULL;
}