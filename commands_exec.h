#ifndef COMMANDS_EXEC_H
#define COMMANDS_EXEC_H

void executeCommand(Command *cmd);
void executeBuiltIn(Command *cmd);
int status;

#endif