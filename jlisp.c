#include "util.h"

/*Function declarations*/
void repl();


/*Main*/
int main()
{
	fprintf(stdout, "JLisp 0.1 Scheme Interpreter\n");
	repl();
	return 0;
}


/*REPL -- main loop*/

void 
repl()
{
	short quit = 0;
	char* line;
	struct token_data* t;
	size_t i;

	while (!quit){
		fprintf(stdout, ">>>");
		line = input(stdin, 10);

		if ((strcmp(line, "quit")) == 0 || (strcmp(line, "exit") == 0))
			break;
		
		else if (strcmp(line, ""))
		{
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