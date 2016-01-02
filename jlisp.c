#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PBUFF  	2048
#define DEBUG	0

struct token_data {
	unsigned elements;
	char** array;
};

char* input(FILE*, size_t);
struct token_data* prep_token_data(unsigned);
struct token_data* tokenize(char*);
void repl();
void parse(char*);
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

struct token_data* prep_token_data(unsigned oLen){
	struct token_data *t;
	size_t i;

	t = (struct token_data*) malloc (sizeof(struct token_data));
//	printf("prep_token_data():\tInitializing height and width\n");

	t->elements = oLen;

	if (DEBUG) printf("prep_token_data():\theight = %d\n", t->elements);

	t->array = (char**) malloc (sizeof(char*) * oLen);
	
	for (i = 0; i < oLen; ++i) t->array[i] = (char*) malloc (oLen);

	if (DEBUG) printf("prep_token_data():\ttoken array prepared\n");

	return t;
}

/* want to take string like "(+ 12 (square 23))" and return array of substrings like ["(", "12", "(", "square", "23", ")", ")"] */
struct token_data *tokenize(char* exp){
	struct token_data* t;
	size_t i = 0, j = 0, k = 0; 
	int size = strlen(exp);
	char tok;

	if (DEBUG) printf("tokenize():\t\texpression received:\t\"%s\"\n",exp);
	if (DEBUG) printf("tokenize():\t\tsizeof expression:\t%d characters\n",size);
	
	t = prep_token_data(size);

	if (DEBUG) fprintf(stdout, "\ntokenize():\t\tStarting parse...\n");
	for (i = 0; i < size; ++i){
//		if (DEBUG) fprintf(stdout, "\ntokenize():\t\titeration:\t%d\n", i+1);
		tok = exp[i];

		if (tok != ' '){
			if (DEBUG) fprintf(stdout, "tokenize():\t\tcurrent token:\t%c\n", tok);

			if ( (isdigit(tok)) || (tok == '-') || (tok == '.') ) {
				if (DEBUG) fprintf(stdout, "tokenize():\t\tNumber token\n");
				do {
					t->array[j][k++] = tok; 
					tok = exp[++i];
				} while( (isdigit(tok)) || (tok == '.') );
				k--; 
				i--;
			} else if ( (isalpha(tok)) || (tok == '_') ) {
				if (DEBUG) fprintf(stdout, "tokenize():\t\tKeyword token\n");
				do {
					t->array[j][k++] = tok;
					tok = exp[++i];
				} while ( (isalpha(tok)) || (tok == '_') || (isdigit(tok)) || (tok == '-'));
				k--; 
				i--;
			} else {
				t->array[j][k] = tok;
			}

			t->array[j] = (char*) realloc (t->array[j], ++k + 1);	//resize the token to fit
			t->array[j][k] = '\0'; 									//terminate string
			if (DEBUG) fprintf(stdout, "tokenize():\t\tt->array[%d] = \"%s\"\n", j, t->array[j]);
			j++;													//increment outer index
			k=0;													//reset the inner index
		}
	}

	t->array = (char**) realloc (t->array, j*sizeof(char*));
	t->elements = j;
	return t;
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