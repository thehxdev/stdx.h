CC ?= cc
CFLAGS := -std=c99 -Wall -Wextra -Wno-unused-result -Og -ggdb
LDFLAGS :=

BIN := a.out

$(BIN): test/test.c src/stdx.h
	$(CC) $(CFLAGS) -o $(BIN) test/test.c

clean:
	rm -rf test
