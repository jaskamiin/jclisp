#include "util.h"

double stone (char* _s0){
	return _s0[0] == '-' ? -atof(++_s0) : atof(_s0);
}

struct token_data* prep_token_data(unsigned oLen){
	struct token_data *t;
	size_t i;
	t = (struct token_data*) malloc (sizeof(struct token_data));
	t->elements = oLen;
	t->array = (char**) malloc (sizeof(char*) * oLen);
	for (i = 0; i < oLen; ++i) t->array[i] = (char*) malloc (oLen);
	return t;
}

/* want to take string like "(+ 12 (square 23))" and return array of substrings like ["(", "12", "(", "square", "23", ")", ")"] */
struct token_data *tokenize(char* exp){
	struct token_data* t;
	size_t i = 0, j = 0, k = 0; 
	int size = strlen(exp);
	char tok;
	
	t = prep_token_data(size);

	for (i = 0; i < size; ++i){
		tok = exp[i];

		if (tok != ' '){
			if ( (isdigit(tok)) || (tok == '-') || (tok == '.') ) {
				do {
					t->array[j][k++] = tok; 
					tok = exp[++i];
				} while( (isdigit(tok)) || (tok == '.') );
				k--; 
				i--;
			} else if ( (isalpha(tok)) || (tok == '_') ) {
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
			j++;													//increment outer index
			k=0;													//reset the inner index
		}
	}

	t->array = (char**) realloc (t->array, j*sizeof(char*));
	t->elements = j;
	return t;
}