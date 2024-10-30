# Makefile for compiling bsearch_serial with chrono timing library

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -O3

# Target executable
TARGET = parteA

# Source files
SRC = parteA.c chrono.c

# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Linking target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)

# Run the program with a default size argument
run: $(TARGET)
	./$(TARGET) 1000000  # Default test with 1 million elements

.PHONY: all clean run
