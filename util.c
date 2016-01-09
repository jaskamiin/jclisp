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

	return f1 || f2 ? 1 : 0; 
}


/*	
*	Used after a token is confirmed to be a number - checks for fractional part 
*/
short
isDecimal (char* _s0)
{
	return (strchr(_s0, '.') == NULL) ? 0 : 1;
}


/*	
*	Tests if the keyword is either a unary or boolean operator or function name 
*
*	TODO: Not very clean. Maybe find a better way to implement this.
*/
short
isOperatorOrFunctionKeyword(char* _s0)
{
	char token;
	unsigned len = strlen(_s0);

	if (len == 1){
		token = _s0[0];
		if (token == '*') return 1;
		if (token == '/') return 2;
		if (token == '+') return 3;
		if (token == '-') return 4;
	} else {
		return 5;
	}

	return 0;
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
*	TODO: 
*		- Make sure changes from var operator are reflected here.
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
*	Create abstract syntax tree from token list. 
*
*	TODO:
*		- determine what return type should be
*		- needs to evaluate function keywords
*		- handle division by 0 error
*		- move evaluation to its own function
*/
void 
parse(struct token_data* tokens)
{
	size_t 			*osIdx, *esIdx;
	unsigned 		tokenCount;
	struct stack 	exprStack;
	struct stack 	opStack;

	tokenCount 		= tokens->elements;
	exprStack.data 	= (struct var*) malloc (tokenCount * sizeof(struct var));
	opStack.data 	= (struct var*) malloc (tokenCount * sizeof(struct var));
	esIdx 			= &exprStack.idx;
	osIdx 			= &opStack.idx;

	esIdx = osIdx = 0;

	size_t 	i;
	short 	opType;
	for (i = 0; i < tokenCount; ++i)
	{
		printf("Token is %s.\n", tokens->array[i]);
		if ( tokens->array[i][0] == '(' ){
			printf("Token is open parentheses. adding to opStack.\n");
			opStack[osIdx].type = OPERATOR;
			opStack[osIdx].precedence = PAR;
			opStack[osIdx].operator = "(\0";

			printf("opStack[%i] added: %c. ", osIdx, opStack[osIdx].operator);
			osIdx++;
			printf("opStack now at %i\n", osIdx);

		} else if ( isNumber(tokens->array[i]) ){
			printf("Token is a number %s. adding to expStack.\n", tokens->array[i]);

			if (isDecimal(tokens->array[i])){
				exprStack[esIdx].type = REAL;
				exprStack[esIdx].real = (double) stone(tokens->array[i]);
			} else {
				exprStack[esIdx].type = INT;
				exprStack[esIdx].integer = (int) stone(tokens->array[i]);				
			}

			esIdx++;

		} else if ((opType = (short)isOperatorOrFunctionKeyword(tokens->array[i])) != 0) {
			printf("Token is an operator: %s. Determining operator type.\n", tokens->array[i]);
			struct var currTok;

			switch (opType){
				case 1: // mul
					printf("Operator is multiplication.\n");
					currTok.operator = "*";
					currTok.precedence = MUL;
					currTok.order = LTR;
					break;
				case 2: //div
					printf("Operator is division.\n");
					currTok.operator = "/";
					currTok.precedence = DIV;
					currTok.order = LTR;
					break;
				case 3: //add
					printf("Operator is addition.\n");
					currTok.operator = "+";
					currTok.precedence = ADD;
					currTok.order = LTR;
					break;
				case 4: //sub
					printf("Operator is subtraction.\n");
					currTok.operator = "-";
					currTok.precedence = SUB;
					currTok.order = LTR;
					break;
				case 5: //function
				{
					printf("Operator is a function keyword.\n");
					size_t _t = sizeof(tokens->array[i]);
					if (_t > 254){
						strncpy(currTok.keyword, tokens->array[i], 254);
						currTok.keyword[255] = '\0';
					} else {
						strncpy(currTok.keyword, tokens->array[i], _t);
					}
					currTok.precedence = FUN;
					currTok.order = RTL;
					break;
				}
			}


			/*This part is a really clumsy glue-job. Will fix later 
			* when I can think about it for a longer time. I'm quite
			* ashamed of the poor style, actually. Oops.*/
			printf("comparing opStack[%i] = %c precedence: %i", osIdx-1, opStack[osIdx-1].operator, opStack[osIdx-1].precedence);
			printf(" with currTok = %c precedence: %i\n", currTok.operator, currTok.precedence);
			while (currTok.precedence < opStack[osIdx-1].precedence)
			{
				printf("Token %c precedence is %i -- current opStack %c precedence is %i\n", currTok.operator, currTok.precedence, opStack[osIdx-1].operator, opStack[osIdx-1].precedence);

				struct var _e1, _e2, result;
				enum bool isDouble1, isDouble2;
				char* 	op;
				double 	_d1, _d2;

				_e1 		= opStack[osIdx-1];
				_e2 		= opStack[osIdx-2];
				op 			= currTok.operator;

				/*Get type of first operand*/
				switch (_e1.type)
				{
					case REAL:
						printf("Operand 1 is real\n");
						_d1 = _e1.real;
						isDouble1 = true;
						break;
					case INT:
						printf("Operand 1 is integer\n");
						_d1 = _e1.integer;
						isDouble1 = false;
						break;
					case STRING:
					case OPERATOR:
					case CHAR:
					case KEYWORD:
						break;
				}

				/*Get type of second operand*/
				switch(_e2.type)
				{
					case REAL:
						printf("Operand 2 is real\n");
						_d2 = _e1.real;
						isDouble2 = true;
						break;
					case INT:
						printf("Operand 2 is integer\n");
						_d2 = _e2.integer;
						isDouble2 = false;
						break;
					case STRING:
					case OPERATOR:
					case CHAR:
					case KEYWORD:
						break;
				}

				/*If one of the operands is a double, so is the result*/
				if ( isDouble1 || isDouble2 )
				{ 
					result.type = REAL;

					switch (op) 
					{
						case '+':
							result.real = _d1 + _d2;
							break;
						case '-':
							result.real = _d1 - _d2;
							break;
						case '*':
							result.real = _d1 * _d2;
							break;
						case '/':
							if (_d2 != 0)
								result.real = _d1 / _d2;
							break;
					}

				} else { 
					result.type = INT;

					switch (op) 
					{
						case '+':
							result.integer = (int)(_d1 + _d2);
							break;
						case '-':
							result.integer = (int)(_d1 - _d2);
							break;
						case '*':
							result.integer = (int)(_d1 * _d2);
							break;
						case '/':
							if (_d2 != 0)
								result.integer = (int)(_d1 / _d2);
							break;
					}
				}

				exprStack[esIdx++] = result;
			}

			opStack[osIdx++] = currTok;

		} else if (tokens->array[i][0] == ')') {
			double e1, e2;
			while (opStack[osIdx].operator != "("){
				switch (opStack[osIdx].type){
					case REAL:
						e1 = 
						break;
					case INT:
						break;
					case KEYWORD:
					case STRING:
					case CHAR:
						break;	
				}
			}

		}

	}

	free (exprStack.data);
	free (opStack.data);
}