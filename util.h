#ifndef UTIL_H
#define UTIL_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef enum { INT, REAL, CHAR, STRING, KEYWORD, OPERATOR } var_type;
typedef enum { LTR, RTL } order;
typedef enum { MUL = 3, DIV = 3, ADD = 4, SUB = 4 } precedence;

struct token_data {
	unsigned elements;
	char** array;
};

struct type_data{
	var_type type;
	precedence prec;
	union {
		int    	integer;
		double 	real;
		char 	character;
		char* 	string;
		char 	keyword[40];
		char 	operator;
	};
};

char* 	input (FILE*, size_t);
double 	stone (char*);
short 	isNumber (char*);
short 	isDecimal (char*);

struct token_data* prep_token_data(unsigned);
struct token_data* tokenize(char*);

void parse(struct token_data*);

#endif