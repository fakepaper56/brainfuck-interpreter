#include "bf_interpreter.h"

#define BUF_SIZE 128
#define RIGHT_LIMIT 64
#define LEFT_LIMIT -64
#define fatal(...)                \
do {                              \
	fprintf(stderr, __VA_ARGS__); \
	exit(EXIT_FAILURE);           \
} while(0)                        \

int main(int argc, char **argv) {
	int fd;
	char buf[BUF_SIZE];
	
	// Format : bfi [file]
	if (argc != 2)
		fatal("bf_interpreter: no input file");

	if ((fd = open(argv[1], O_RDONLY)) < 0)
		fatal("bf_interpreter: %s: %s\n", argv[1], strerror(errno));

	// write to buffer, and check its size
	ssize_t res = read(fd, buf, BUF_SIZE);
	if (res < 0)
		fatal("bf_interpreter: %s: %s", argv[1], strerror(errno));

	if (128 == res) 
		fatal("bf_interpreter: no support for string larger than 127 byte");

	close(fd);
	
	src = buf;
	
	// do work
	interpreter();
	return 0;
}

int next() {
	while(1) { // if some char we do not recognized, skip it.
		switch (*src++) {
		case '\0': return HALT;
		case '>' : return INCP;
		case '<' : return DECP;
		case '+' : return INCV;
		case '-' : return DECV;
		case '.' : return PUT;
		case ',' : return GET;
		case '[' : return LW;
		case ']' : return RW;
		}
	}
}

void interpreter() {
	
	char *ptr = (char *)calloc(127,sizeof(char)) + 64;
	int axis = 0;
	static void *dispatchObjs[] = {
	[HALT] = &&halt, 
	[INCP] = &&inc_pointer,
	[DECP] = &&dec_pointer,
	[INCV] = &&inc_value,
	[DECV] = &&dec_value,
	[PUT]  = &&put_char,
	[GET]  = &&get_char,
	[LW]   = &&left_bracket,
	[RW]   = &&right_bracket};

#define dispatch() goto *dispatchObjs[next()]

	dispatch();
	while (1) {
	halt:
		break;
	inc_pointer:
		if (axis++ == RIGHT_LIMIT)
			fatal("The bf_interpreter can't affor too much right shrift");
		++ptr;
		dispatch();
	dec_pointer:
		if (axis-- == LEFT_LIMIT)
			fatal("The bf_interpreter can't affor too much left shrift");
		--ptr;
		dispatch();
	inc_value:
		++*ptr;
		dispatch();
	dec_value:
		--*ptr;
		dispatch();
	put_char:
		putchar(*ptr);
		dispatch();
	get_char:
		*ptr = getchar();
		dispatch();
	left_bracket:
		leftBracket(ptr);
		dispatch();
	right_bracket:
		rightBracket(ptr);
		dispatch();
	}
}

void leftBracket(char *ptr){
	if (*ptr == '\0') {
		src = strchr(src, ']');
		if (src == NULL)
			fatal("syntax error: expected ]");
		++src;
		return;
	}
	
	if (top == 3)
		fatal("syntax error: cna't afford 4 nested while");
	bracketStack[++top] = src;
}

void rightBracket(char *ptr) {
	if (top == -1)
		fatal("syntax error: too many ]");
	if (*ptr == '\0') {
		--top;
		return;
	}
	src = bracketStack[top];
}
