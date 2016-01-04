#include "util.h"

/*
*	Take string and convert it to signed double.
*  	Result can be casted to integer if wanted.
*	
*	TODO: Try to make explicit casting necessary for other types than double.
*/
double 
stone (char* _s0)
{
	return _s0[0] == '-' ? -atof(++_s0) : atof(_s0);
}



/*	
*	Tests to see if either the token starts with a digit or negative sign. If 
*	a negative sign, it then checks if the next digit is a digit. 
*
*	TODO: Not very clean. Maybe find a better way to implement this.
*/
short
isNumber (char* _s0)
{
	short f1, f2;

	f1 = isdigit(_s0[0]);
	f2 = ( _s0[0] =='-' && isdigit(_s0[1]) );

	return f1 && f2 ? 1 : 0; 
}


short
isDecimal (char* _s0)
{
	return (strchr(_s0, '.') == NULL) ? 0 : 1;
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
struct token_data* 
prep_token_data(unsigned N)
{
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
struct token_data* 
tokenize(char* exp)
{
	struct token_data* t;
	size_t i = 0, j = 0, k = 0; 
	int size = strlen(exp);
	char tok;
	
	t = prep_token_data(size);

	for (i = 0; i < size; ++i)
	{
		tok = exp[i];

		if (tok != ' ')
		{

			if ((isdigit(tok)) || (tok == '-') || (tok == '.')) 
			{
				do 
				{
					t->array[j][k++] = tok; 
					tok = exp[++i];
				} while((isdigit(tok)) || (tok == '.'));
				k--; i--;
			} 

			else if ((isalpha(tok)) || (tok == '_')) 
			{
				do 
				{
					t->array[j][k++] = tok;
					tok = exp[++i];
				} while ((isalpha(tok)) || (tok == '_') || (isdigit(tok)) || (tok == '-'));
				k--; i--;
			} 

			else t->array[j][k] = tok;

			t->array[j] = (char*) realloc (t->array[j], ++k + 1);	/*resize the token to 'fit'*/
			t->array[j][k] = '\0'; 
			j++;
			k=0;
		}
	}

	t->array = (char**) realloc (t->array, j * sizeof(char*));
	t->elements = j;
	return t;
}


/*
*	Take arbitrary size input from specified stream (stdin, usually) and store
*	in a dynamically reallocated string, which is returned as the result.
*/
char* 
input(FILE* in, size_t size)
{
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


/*
*	Create abstract syntax tree from token list. 
*
*	TODO:
*		- determine what return type should be
*		- only handles token being a number so far.
*/
void 
parse(struct token_data* tokens)
{
	size_t i, tok_len, esIdx, osIdx;
	unsigned numToks = tokens->elements;

	struct type_data exprStack[(const unsigned) numToks];
	struct type_data opStack[(const unsigned) numToks];

	esIdx = osIdx = 0;

	for (i = 0; i < numToks; ++i)
	{

		if ( tokens->array[i][0] == '(' ){

			opStack[osIdx].type = OPERATOR;
			opStack[osIdx].operator = '(';
			osIdx++;

		} else if ( isNumber(tokens->array[i]) ){

			if (isDecimal(tokens->array[i])){
				exprStack[esIdx].type = REAL;
				exprStack[esIdx].real = (double) stone(tokens->array[i]);
			} else {
				exprStack[esIdx].type = INT;
				exprStack[esIdx].integer = (int) stone(tokens->array[i]);				
			}

			esIdx++;

		} else if () {

			

		}

	}
}