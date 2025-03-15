# compiler
CC = gcc

# compiler flags
CFLAGS = -Wall -Wextra -Werror

# source files
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# exec file
TARGET = poker

# default target
all: $(TARGET)

# link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


# clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# run program
run: $(TARGET)
	./$(TARGET)

# not files
.PHONY: all clean run
