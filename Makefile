.PHONY: all clean

all: main

main: main.o async_http_utils.o async_http_attr.o async_http_callbacks.o async_http_request.o
	gcc -lcurl -Wall -pedantic -o $@ $^

async_http_utils.o: async_http_utils.c async_http_utils.h
	gcc -Wall -pedantic -o $@ -c $<

async_http_attr.o: async_http_attr.c async_http_attr.h async_http_utils.h
	gcc -Wall -pedantic -o $@ -c $<

async_http_callbacks.o: async_http_callbacks.c async_http_callbacks.h async_http_utils.h
	gcc -Wall -pedantic -o $@ -c $<

async_http_request.o: async_http_request.c async_http_request.h async_http_utils.h
	gcc -Wall -pedantic -o $@ -c $<

main.o: main.c async_http_attr.h async_http_utils.h
	gcc -Wall -pedantic -o $@ -c $<

clean:
	rm -f *.o main