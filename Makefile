shared:
	gcc -c -Wall -Werror -fpic list.c -o list.o
	gcc -shared -o list.so list.o
test:
	@gcc tests.c list.c tests/positive_tests.c -o testsexec
	-@./testsexec
	@rm -f testsexec