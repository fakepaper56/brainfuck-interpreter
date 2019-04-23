CC = gcc
OBJ = bf_interpreter.o

all: bfi

%.o: %.c
	$(CC) -c -o $@ $<

bfi: $(OBJ)
	$(CC) -o bfi $^

clean:
	rm -f bfi $(OBJ)
