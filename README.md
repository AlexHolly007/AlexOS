# AlexOS
While not an Operating System, this is a replica of an interactive shell including many similar features:
- process forking and execution
- Background processes
- I/O redirection
- Signal handeling (crtl c, child process return)
- Variable expansion
- Modes to be activated<br>

<a href="(https://youtu.be/wBHxsVFhX0U?si=6QXD-rFHLSz8QI36)"><img width='485' src="https://github.com/user-attachments/assets/bd4b78e5-93e4-4352-a596-e44241bea0b1"></a><br>

What was so challenging but valuable from this project was that it was designed the same way as a real shell. For example when a linux shell is running a command or program, it will fork/copy the main process into two, and then change the second process to fit the task that was called. This was done the same way using C in this project along with the signal handeling.

run 'make' to compile, see Makefile for more information.
