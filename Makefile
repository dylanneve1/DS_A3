###############################
#        DS_A2 21364333       #
#   Makefile for Assignment   #
###############################

# Compiler
CC = gcc
CFLAGS = -g -Wall

######################
#      TARGETS       #
######################

# Both targets with one command
all: task1 task2

# Target for task1
task1: task1.o bst.o
	@echo "Linking task1..."
	$(CC) $(CFLAGS) -o task1 task1.o bst.o

# Target for task2
task2: task2.o bstdb.o database.o listdb.o profiler.o
	@echo "Linking t2_test..."
	$(CC) $(CFLAGS) -o task2 task2.o bstdb.o database.o listdb.o profiler.o

######################
#    BUILD RULES     #
######################

# Compile task1 object
task1.o: ./src/task1.c ./src/bst.h
	@echo "Compiling task1.c..."
	$(CC) $(CFLAGS) -c ./src/task1.c

# Compile bst object
bst.o: ./src/bst.c ./src/bst.h
	@echo "Compiling bst.c..."
	$(CC) $(CFLAGS) -c ./src/bst.c

# Compile task2 object
task2.o: ./src/task2.c ./src/db/database.h ./src/db/profiler.h
	@echo "Compiling task2.c..."
	$(CC) $(CFLAGS) -c ./src/task2.c

# Compile database object
database.o: ./src/db/database.c ./src/bstdb.h ./src/db/database.h ./src/db/listdb.h
	@echo "Compiling database.c..."
	$(CC) $(CFLAGS) -c ./src/db/database.c

# Compile bstdb object
bstdb.o: ./src/bstdb.c ./src/bstdb.h
	@echo "Compiling bstdb.c..."
	$(CC) $(CFLAGS) -c ./src/bstdb.c

# Compile listdb object
listdb.o: ./src/db/listdb.c ./src/db/listdb.h
	@echo "Compiling listdb.c..."
	$(CC) $(CFLAGS) -c ./src/db/listdb.c

# Compile profiler object
profiler.o: ./src/db/profiler.c ./src/db/profiler.h ./src/db/database.h
	@echo "Compiling profiler.c..."
	$(CC) $(CFLAGS) -c ./src/db/profiler.c

######################
#     CLEAN RULES    #
######################

# Clean up all object files and executables
clean:
	@echo "Cleaning up..."
	rm -f *.o task1 task2

######################
#    PHONY TARGETS   #
######################

.PHONY: all clean
