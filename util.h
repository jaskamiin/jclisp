#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>

struct token_data {
	unsigned elements;
	char** array;
};

double stone (char*);
struct token_data* prep_token_data(unsigned);
struct token_data* tokenize(char*);
void parse(char*);

#endif