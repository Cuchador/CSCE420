
# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# Builds .o files, then builds executable, then runs the executable
all: make run

make: blocksworld.o blocksworld

# BUILDS ALL EXECUTABLES
blocksworld: blocksworld.o State.o Node.o
	$(CC) $(CFLAGS) -o blocksworld blocksworld.o State.o Node.o


# BUILDS ALL .o FILES
blocksworld.o: blocksworld.cpp State.h Node.h
	$(CC) $(CFLAGS) -c blocksworld.cpp

State.o: State.h
Node.o: Node.h

# RUNS ALL EXECUTABLES WITH COMMAND LINE ARGUMENTS
# ./exe <filename> [-H ]
run:
	./blocksworld 

# CLEARS BUILT FILES
clean:
	$(RM) blocksworld blocksworld.o State State.o Node Node.o