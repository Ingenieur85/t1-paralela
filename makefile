# Makefile for compiling both parteA and partB with chrono timing library

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -O3

# Target executables
TARGETS = parteA parteB

# Common object files
COMMON_OBJ = chrono.o

# Program-specific source and object files
PARTA_SRC = parteA.c
PARTB_SRC = parteB.c
PARTA_OBJ = $(PARTA_SRC:.c=.o)
PARTB_OBJ = $(PARTB_SRC:.c=.o)

# Default target
all: $(TARGETS)

# Linking target executables
parteA: $(PARTA_OBJ) $(COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

parteB: $(PARTB_OBJ) $(COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(COMMON_OBJ) $(PARTA_OBJ) $(PARTB_OBJ) $(TARGETS)

.PHONY: all clean