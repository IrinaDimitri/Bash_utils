CC = gcc
CFLAG = -std=c11 -Wall -Werror -Wextra
EXECUTABLE = cat

all: cat

cat: cat.o
	$(CC) $(CFLAG) -o $(EXECUTABLE) cat.o

cat.o: cat.c cat.h
	$(CC) $(CFLAG) -c cat.c

test:
	bash test_func_cat.sh

style: 
	clang-format -style=Google -n *.c *.h

clean:
	rm cat
	rm *.o

rebuild:
	clean all