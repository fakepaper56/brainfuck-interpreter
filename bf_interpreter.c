#include "bf_interpreter.h"
int main(int argc, char **argv) {
	FILE *fp;
	char buf[128];
	
	// Format : bfi [file]
	if (argc != 2){
		puts("no input file");
		return 1;
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("%s: no such file\n",argv[1]);
		return 1;
	}

	// write to buffer, and check its size
	size_t size = fread(buf,1,1024,fp);
	if (128 == size) {
		printf("the interpreter does not support string larger than 127 byte");
		return 1;
	}

	src = buf;
	
	// do work
	interpreter();
	return 0;
}

int next() {
	while(1) { // if some char we do not recognized, skip it.
		switch (*src++) {
		case '\0':
			return HALT;
		case '>' :
			return INCP;
		case '<' : 
			return DECP;
		case '+' : 
			return INCV;
		case '-' : 
			return DECV;
		case '.' :
			return PUT;
		case ',' :
			return GET;
		case '[' : 
			return LW;
		case ']' : 
			return RW;
		}
	}
}

void interpreter() {
	char *ptr = (char *)calloc(128,sizeof(char));
	static void *dispatchObjs[] = {&&halt, &&inc_pointer,&&dec_pointer,&&inc_value,&&dec_value,
								   &&put_char,&&get_char,&&left_bracket,&&right_bracket};

#define dispatch() goto *dispatchObjs[next()]

	dispatch();
	while (1) {
	halt:
		break;
	inc_pointer:
		++ptr;
		dispatch();
	dec_pointer:
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
			syntaxErr("expected more ]");
		++src;
		return;
	}
	
	if (top == 3)
		syntaxErr("the interpreter cna't afford 4 nested while");
	bracketStack[++top] = src;
}

void rightBracket(char *ptr) {
	if (top == -1)
		syntaxErr("too many ]");
	if (*ptr == '\0') {
		--top;
		return;
	}
	src = bracketStack[top];
}

void syntaxErr(char *err) {
	printf("syntax error:%s\n",err);
	exit(1);
}
