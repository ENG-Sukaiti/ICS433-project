CC = gcc
CFLAGS = -O2 -Wall -Wextra -g

all: test_allocator

test_allocator: test_allocator.o my_alloc.o metrics.o
	$(CC) $(CFLAGS) -o $@ $^

test_allocator.o: test_allocator.c my_alloc.h
	$(CC) $(CFLAGS) -c $<

my_alloc.o: my_alloc.c my_alloc.h
	$(CC) $(CFLAGS) -c $<

metrics.o: metrics.c my_alloc.h
	$(CC) $(CFLAGS) -c $<

test: all
	./test_allocator

clean:
	rm -f *.o test_allocator
