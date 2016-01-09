#ifndef UTIL_H
#define UTIL_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum bool { true, false };

typedef enum { INT, REAL, CHAR, STRING, KEYWORD, OPERATOR } var_type;
typedef enum { LTR, RTL } var_order;
typedef enum { PAR = 0, FUN = 1, MUL = 3, DIV = 3, ADD = 4, SUB = 4 } var_precedence;


struct token_data 
{
	unsigned elements;
	char** array;
};

/*TODO: merge keyword and operator into one*/
struct var
{
	var_type type;
	var_order order;
	var_precedence precedence;
	union {
		int    	integer;
		double 	real;
		char 	character;
		char* 	string;
		char 	operator[255];
	};
};

struct stack
{
	size_t idx;
	struct var* data;
};


char* 	input (FILE*, size_t);
double 	stone (char*);
short 	isNumber (char*);
short 	isDecimal (char*);

struct 	token_data* prep_token_data(unsigned);
struct 	token_data* tokenize(char*);

void 	parse(struct token_data*);


#endif