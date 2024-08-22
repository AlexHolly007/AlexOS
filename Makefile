CC = gcc
CFLAGS = --std=gnu99 -Wall

# Source files
SRCS = smallsh_main.c parsing_inputs.c commands_exec.c

# Object files
OBJS = $(SRCS:.c=.o)

EXEC = smallsh

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)
