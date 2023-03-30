# Group Project 1 - Kalista Fong and Ruth Shiferaw

GCC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

#test: puzzle_tests
#	./puzzle_tests
# REAL MAKEFILE !!! 
# Get a list of all the source files in the folder



puzzle: puzzle.o ppm_io.o puzzle_funcs.o 
	$(GCC) -g -o puzzle puzzle.o ppm_io.o puzzle_funcs.o 
	
puzzle.o: puzzle.c puzzle.h ppm_io.h
	$(GCC) -c puzzle.c $(CFLAGS)

ppm_io.o: ppm_io.c ppm_io.h
	$(GCC) -c ppm_io.c $(CFLAGS)

puzzle_funcs.o: puzzle_funcs.c puzzle.h
	$(GCC) -c puzzle_funcs.c $(CFLAGS)

test: puzzle_tests.o puzzle.o
	$(GCC) -o puzzle_tests puzzle_tests.o puzzle.o

puzzle_tests.o: puzzle_tests.c puzzle.h 
	$(GCC) -c puzzle_tests.c $(CFLAGS)

clean: 
	rm -f *.o puzzle test *~