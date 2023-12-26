shared:
	gcc -c -Wall -Werror -fpic list.c -o list.o
	gcc -shared -o list.so list.o