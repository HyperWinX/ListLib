shared:
	gcc -c -Wall -Werror -fpic list.c -o list.o
	gcc -shared -o list.so list.o
tests:
	@gcc tests.c list.c -O2 -march=native -o tests
	-@./tests
	@rm -f tests