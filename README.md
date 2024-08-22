# AlexOS
This is a replica of an interactive shell including many similar features:
 *process forking and execution
 *background capability
 *I/O redirection
 *Signal handeling (crtl c, child process return)
 *Variable expansion.
 *Some special modes that can be activated

What was so challenging but valuable from this project was that it was designed the same way as a real shell. For example when a linux shell is running a command or program, it will fork, or copy the main process into two, and then change the second process to fit the task that was called.

run 'make' to compile, see Makefile for more information.
