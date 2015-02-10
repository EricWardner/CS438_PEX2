shell: shell.c shell.h list.o makefile
	gcc shell.c list.o -o shell -Wall

list: list.c list.h makefile
	gcc -c list.c -o list.o -Wall

clean:
	rm shell
	rm -f *.o
	rm -f *~
