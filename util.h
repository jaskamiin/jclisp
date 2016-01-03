#ifndef UTIL_H
#define UTIL_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef enum { INT, REAL, CHAR, STRING, KEYWORD, OPERATOR } var_type;

struct token_data {
	unsigned elements;
	char** array;
};

struct type_data{
	var_type type; 
	union {
		int    	integer;
		double 	real;
		char 	character;
		char* 	string;
		char 	keyword[40];
		char 	operator;
	};
};

char* input(FILE*, size_t);
double stone (char*);
struct token_data* prep_token_data(unsigned);
struct token_data* tokenize(char*);
void parse(struct token_data*);

#endif