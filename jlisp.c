#include "util.h"

char* input(FILE*, size_t);
void repl();
void eval();

int main(){
	fprintf(stdout, "JLisp 0.1 Scheme Interpreter\n");
	repl();
	return 0;
}

char* input(FILE* in, size_t size){
	char* str = NULL;
	int c;
	size_t len = 0;
	
	str = (char*)realloc(str, size);
	if (!str) return str;

	while ((c=fgetc(in)) != EOF && c != '\n'){
		str[len++] = c;
		if (len == size){
			str = (char*)realloc(str, size += 16);
			if (!str) return str;
		}
	}
	str[len++] = '\0';

	return (char*)realloc(str, len);
}

void repl(){
	short quit = 0;
	char* line;
	struct token_data* t;
	size_t i;

	while (!quit){
		fprintf(stdout, ">>>");
		line = input(stdin, 10);
		if (strcmp(line, "") != 0){
			//does not freeing this every time cause a memory leak? crashes when I try.
			t = tokenize(line);

			printf("[");
			for (i = 0; i < t->elements; ++i){
				printf("[\"%s\"]", t->array[i]);
				if (i < t->elements-1) printf(", ");
			}
			printf("]\n");
		}
	}

	for (i = 0; i < t->elements; ++i) free(t->array[i]);
	free(t->array);
	free(t);
	free(line);
}