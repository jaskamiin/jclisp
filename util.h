#ifndef UTIL_H
#define UTIL_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct token_data {
	unsigned elements;
	char** array;
};

double stone (char*);
struct token_data* prep_token_data(unsigned);
struct token_data* tokenize(char*);
void parse(char*);

#endif