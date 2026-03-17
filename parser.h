#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// Function prototypes
int parse(TokenList *token_list);
void print_parse_result(int success);

#endif
