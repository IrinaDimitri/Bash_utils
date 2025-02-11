CC = gcc
CFLAG = -std=c11 -Wall -Werror -Wextra
EXECUTABLE = grep

all: grep 

grep: grep.o
	$(CC) $(CFLAG) -o $(EXECUTABLE) grep.o

grep.o: grep.c grep.h
	$(CC) $(CFLAG) -c grep.c

test:
	bash test_func_grep.sh

style: 
	clang-format -style=Google -n *.c *.h

clean:
	rm grep
	rm *.o

rebuild:
	clean all