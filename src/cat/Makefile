CC= gcc
CFLAGS=-Wall -Werror -Wextra
EXECUTABLE=s21_cat
SOURSES= s21_cat.c
OBJECTS=$(SOURSES:.cpp=.o)

all: s21_cat

s21_cat: s21_cat.o
	$(CC) $(CFLAGS) s21_cat.o -o $(EXECUTABLE)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@ 

clean: 
	rm *.o
	rm s21_cat

rebuild: clean all
