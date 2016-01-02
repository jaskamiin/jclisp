#include "util.h"

/*
*	Take string and convert it to signed double.
*  	Result can be casted to integer if wanted.
*	
*	TODO: Try to make explicit casting necessary for other types than double.
*/
double stone (char* _s0){
	return _s0[0] == '-' ? -atof(++_s0) : atof(_s0);
}


/*
*	Return a token structure with a 2D square array, dimension equal to input.
*
*	Since for an input of length N, there will be x <= N tokens, each with
*	length y <= N. Thus a square array of size N is created and memory 
*	is reallocated later in tokenize() to get rid of empty space.
*	
*	TODO: Try to reimplement this without malloc() for speed
*/
struct token_data* prep_token_data(unsigned N){
	struct token_data *t;
	size_t i;

	t = (struct token_data*) malloc (sizeof(struct token_data));
	t->array = (char**) malloc (sizeof(char*) * N);

	for (i = 0; i < N; ++i) 
		t->array[i] = (char*) malloc (N);

	t->elements = N;

	return t;
}


/* 	
*	Take string and return array of its substrings by 'classification'; i.e.:
*	parentheses and operators, numbers, and keywords (proc names, vars, etc).
*	Whitespace is ignored.
*	
*	TODO: Nothing
*/
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
				k--; i--;
			} else if ( (isalpha(tok)) || (tok == '_') ) {
				do {
					t->array[j][k++] = tok;
					tok = exp[++i];
				} while ( (isalpha(tok)) || (tok == '_') || (isdigit(tok)) || (tok == '-'));
				k--; i--;
			} else {
				t->array[j][k] = tok;
			}

			t->array[j] = (char*) realloc (t->array[j], ++k + 1);	/*resize the token to 'fit'*/
			t->array[j][k] = '\0'; 
			j++;
			k=0;
		}
	}

	t->array = (char**) realloc (t->array, j*sizeof(char*));
	t->elements = j;
	return t;
}