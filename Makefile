shared:
	gcc -c -Iinclude -Wall -Wextra -fpic src/list.c -o list.o
	gcc -shared -o list.so list.o
tests:
	@gcc -Iinclude test/tests.c src/list.c -o testsexec -Wall -Werror
	-@./testsexec
	@rm -f testsexec
