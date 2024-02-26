shared:
	gcc -c -Wall -Werror -fpic list.c -o list.o
	gcc -shared -o list.so list.o
test:
	@gcc tests.c list.c -o testsexec -Wall -Werror
	-@./testsexec
	@rm -f testsexec
