CC= gcc
CFLAGS=-Wall -Werror -Wextra
EXECUTABLE=s21_grep
SOURSES= s21_grep.c
OBJECTS=$(SOURSES:.cpp=.o)

all: s21_grep

s21_grep: s21_grep.o
	$(CC) $(CFLAGS) s21_grep.o -o $(EXECUTABLE)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@ 

clean: 
	rm *.o
	rm s21_grep

rebuild: clean all
