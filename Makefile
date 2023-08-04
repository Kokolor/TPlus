COMPILER = t+

SRC = $(COMPILER).c
OBJ = $(COMPILER).o

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =

all: $(COMPILER)

$(COMPILER): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(COMPILER)
	cp $(COMPILER) /usr/local/bin/

clean:
	rm -f $(OBJ) $(COMPILER)

.PHONY: all install clean

