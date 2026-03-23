#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "symbol_table.h"
#include "intermediate_code.h"

static TokenList *tokens;
static int current_token_index;
static int parse_errors;

static Token* current_token() {
    if (current_token_index < tokens->count) {
        return &tokens->tokens[current_token_index];
    }
    return &tokens->tokens[tokens->count - 1];
}

static Token* token_at(int offset) {
    int idx = current_token_index + offset;
    if (idx >= 0 && idx < tokens->count) {
        return &tokens->tokens[idx];
    }
    return &tokens->tokens[tokens->count - 1];
}

static void advance() {
    if (current_token_index < tokens->count - 1) {
        current_token_index++;
    }
}

static int match(TokenType type) {
    return current_token()->type == type;
}

static int is_type_token(TokenType type) {
    return type == TOKEN_INT || type == TOKEN_FLOAT;
}

static const char* token_type_to_datatype(TokenType type) {
    return type == TOKEN_FLOAT ? "float" : "int";
}

static void expect(TokenType type, const char *error_msg) {
    if (!match(type)) {
        fprintf(stderr, "Syntax Error at line %d: %s\n", current_token()->line_number, error_msg);
        fprintf(stderr, "Expected %s but found %s ('%s')\n",
                get_token_name(type),
                get_token_name(current_token()->type),
                current_token()->lexeme);
        parse_errors++;
    } else {
        advance();
    }
}

static char* parse_expression();
static char* parse_term();
static char* parse_factor();
static void parse_statement(int in_function);
static void parse_statement_list(int stop_on_rbrace, int in_function);

static char* parse_factor() {
    static char result[MAX_LEXEME_LENGTH];

    if (match(TOKEN_IDENTIFIER)) {
        char identifier[MAX_LEXEME_LENGTH];
        strcpy(identifier, current_token()->lexeme);
        advance();

        if (match(TOKEN_LPAREN)) {
            advance();
            int arg_count = 0;
            if (!match(TOKEN_RPAREN)) {
                while (1) {
                    char *arg = parse_expression();
                    char param_line[MAX_LINE_LENGTH];
                    snprintf(param_line, sizeof(param_line), "param %s", arg);
                    add_code(param_line);
                    arg_count++;

                    if (match(TOKEN_COMMA)) {
                        advance();
                    } else {
                        break;
                    }
                }
            }
            expect(TOKEN_RPAREN, "Expected ')' after function call arguments");

            char *temp = generate_temp();
            char code_line[MAX_LINE_LENGTH];
            snprintf(code_line, sizeof(code_line), "%s = call %s, %d", temp, identifier, arg_count);
            add_code(code_line);
            strcpy(result, temp);
            return result;
        }

        if (match(TOKEN_LBRACKET)) {
            advance();
            char *index_expr = parse_expression();
            expect(TOKEN_RBRACKET, "Expected ']' after array index");

            if (lookup_symbol(identifier) == -1) {
                fprintf(stderr, "Error at line %d: Undeclared array '%s'\n",
                        current_token()->line_number, identifier);
                parse_errors++;
            }

            char *temp = generate_temp();
            char code_line[MAX_LINE_LENGTH];
            snprintf(code_line, sizeof(code_line), "%s = %s[%s]", temp, identifier, index_expr);
            add_code(code_line);
            strcpy(result, temp);
            return result;
        }

        if (lookup_symbol(identifier) == -1) {
            fprintf(stderr, "Error at line %d: Undeclared variable '%s'\n",
                    current_token()->line_number, identifier);
            parse_errors++;
        }

        strcpy(result, identifier);
        return result;
    }

    if (match(TOKEN_NUMBER)) {
        strcpy(result, current_token()->lexeme);
        advance();
        return result;
    }

    if (match(TOKEN_LPAREN)) {
        advance();
        char *expr = parse_expression();
        strcpy(result, expr);
        expect(TOKEN_RPAREN, "Expected ')' after expression");
        return result;
    }

    fprintf(stderr, "Syntax Error at line %d: Expected identifier, number, or '('\n",
            current_token()->line_number);
    parse_errors++;
    strcpy(result, "0");
    return result;
}

static char* parse_term() {
    static char result[MAX_LEXEME_LENGTH];
    strcpy(result, parse_factor());

    while (match(TOKEN_MULTIPLY) || match(TOKEN_DIVIDE)) {
        char op = current_token()->lexeme[0];
        advance();

        char *right = parse_factor();
        char *temp = generate_temp();
        char code_line[MAX_LINE_LENGTH];
        snprintf(code_line, sizeof(code_line), "%s = %s %c %s", temp, result, op, right);
        add_code(code_line);
        strcpy(result, temp);
    }

    return result;
}

static char* parse_expression() {
    static char result[MAX_LEXEME_LENGTH];
    strcpy(result, parse_term());

    while (match(TOKEN_PLUS) || match(TOKEN_MINUS)) {
        char op = current_token()->lexeme[0];
        advance();

        char *right = parse_term();
        char *temp = generate_temp();
        char code_line[MAX_LINE_LENGTH];
        snprintf(code_line, sizeof(code_line), "%s = %s %c %s", temp, result, op, right);
        add_code(code_line);
        strcpy(result, temp);
    }

    return result;
}

static char* parse_condition() {
    static char result[MAX_LEXEME_LENGTH];
    char left_expr[MAX_LEXEME_LENGTH];
    strcpy(left_expr, parse_expression());

    if (!(match(TOKEN_EQUAL) || match(TOKEN_NOT_EQUAL) || match(TOKEN_LESS) ||
          match(TOKEN_GREATER) || match(TOKEN_LESS_EQUAL) || match(TOKEN_GREATER_EQUAL))) {
        fprintf(stderr, "Syntax Error at line %d: Expected relational operator in condition\n",
                current_token()->line_number);
        parse_errors++;
        strcpy(result, left_expr);
        return result;
    }

    char op[3];
    strncpy(op, current_token()->lexeme, sizeof(op) - 1);
    op[sizeof(op) - 1] = '\0';
    advance();

    char right_expr[MAX_LEXEME_LENGTH];
    strcpy(right_expr, parse_expression());
    char *temp = generate_temp();
    char code_line[MAX_LINE_LENGTH];
    snprintf(code_line, sizeof(code_line), "%s = %s %s %s", temp, left_expr, op, right_expr);
    add_code(code_line);
    strcpy(result, temp);

    return result;
}

static void parse_print_stmt() {
    advance();
    expect(TOKEN_LPAREN, "Expected '(' after 'print'");

    char *expr = parse_expression();
    char code_line[MAX_LINE_LENGTH];
    snprintf(code_line, sizeof(code_line), "print %s", expr);
    add_code(code_line);

    expect(TOKEN_RPAREN, "Expected ')' after print expression");
    expect(TOKEN_SEMICOLON, "Expected ';' at end of statement");
}

static void parse_assignment_or_call() {
    char name[MAX_LEXEME_LENGTH];
    strcpy(name, current_token()->lexeme);
    advance();

    if (match(TOKEN_LPAREN)) {
        advance();
        int arg_count = 0;
        if (!match(TOKEN_RPAREN)) {
            while (1) {
                char *arg = parse_expression();
                char param_line[MAX_LINE_LENGTH];
                snprintf(param_line, sizeof(param_line), "param %s", arg);
                add_code(param_line);
                arg_count++;

                if (match(TOKEN_COMMA)) {
                    advance();
                } else {
                    break;
                }
            }
        }
        expect(TOKEN_RPAREN, "Expected ')' after function call arguments");
        expect(TOKEN_SEMICOLON, "Expected ';' at end of statement");

        char code_line[MAX_LINE_LENGTH];
        snprintf(code_line, sizeof(code_line), "call %s, %d", name, arg_count);
        add_code(code_line);
        return;
    }

    int is_array_assignment = 0;
    char index_expr[MAX_LEXEME_LENGTH] = {0};

    if (match(TOKEN_LBRACKET)) {
        is_array_assignment = 1;
        advance();
        strcpy(index_expr, parse_expression());
        expect(TOKEN_RBRACKET, "Expected ']' after array index");
    }

    if (lookup_symbol(name) == -1) {
        fprintf(stderr, "Error at line %d: Undeclared variable '%s'\n",
                current_token()->line_number, name);
        parse_errors++;
    } else {
        mark_initialized(name);
    }

    expect(TOKEN_ASSIGN, "Expected '=' in assignment");
    char *expr_result = parse_expression();

    char code_line[MAX_LINE_LENGTH];
    if (is_array_assignment) {
        snprintf(code_line, sizeof(code_line), "%s[%s] = %s", name, index_expr, expr_result);
    } else {
        snprintf(code_line, sizeof(code_line), "%s = %s", name, expr_result);
    }
    add_code(code_line);

    expect(TOKEN_SEMICOLON, "Expected ';' at end of statement");
}

static void parse_declaration() {
    TokenType type_token = current_token()->type;
    const char *datatype = token_type_to_datatype(type_token);
    advance();

    if (!match(TOKEN_IDENTIFIER)) {
        fprintf(stderr, "Syntax Error at line %d: Expected identifier after type\n",
                current_token()->line_number);
        parse_errors++;
        return;
    }

    char var_name[MAX_LEXEME_LENGTH];
    strcpy(var_name, current_token()->lexeme);
    advance();

    if (match(TOKEN_LBRACKET)) {
        advance();
        if (!match(TOKEN_NUMBER)) {
            fprintf(stderr, "Syntax Error at line %d: Expected array size\n",
                    current_token()->line_number);
            parse_errors++;
        } else {
            int array_size = atoi(current_token()->lexeme);
            if (array_size <= 0) {
                fprintf(stderr, "Syntax Error at line %d: Array size must be > 0\n",
                        current_token()->line_number);
                parse_errors++;
            } else if (add_array_symbol(var_name, datatype, array_size) == -1) {
                parse_errors++;
            }
            advance();
        }
        expect(TOKEN_RBRACKET, "Expected ']' after array size");
        expect(TOKEN_SEMICOLON, "Expected ';' at end of declaration");
        return;
    }

    if (add_symbol(var_name, datatype) == -1) {
        parse_errors++;
    }

    if (match(TOKEN_ASSIGN)) {
        advance();
        char *expr_result = parse_expression();
        mark_initialized(var_name);

        char *endptr = NULL;
        double value = strtod(expr_result, &endptr);
        if (endptr != expr_result && *endptr == '\0') {
            update_symbol_value(var_name, value);
        }

        char code_line[MAX_LINE_LENGTH];
        snprintf(code_line, sizeof(code_line), "%s = %s", var_name, expr_result);
        add_code(code_line);
    }

    expect(TOKEN_SEMICOLON, "Expected ';' at end of declaration");
}

static void parse_block(int in_function) {
    expect(TOKEN_LBRACE, "Expected '{' to start block");
    parse_statement_list(1, in_function);
    expect(TOKEN_RBRACE, "Expected '}' to close block");
}

static void parse_if_statement(int in_function) {
    advance();
    expect(TOKEN_LPAREN, "Expected '(' after 'if'");
    char *condition = parse_condition();
    expect(TOKEN_RPAREN, "Expected ')' after if condition");

    char false_label[16];
    char end_label[16];
    strcpy(false_label, generate_label());
    strcpy(end_label, generate_label());

    char line[MAX_LINE_LENGTH];
    snprintf(line, sizeof(line), "ifFalse %s goto %s", condition, false_label);
    add_code(line);

    parse_block(in_function);

    if (match(TOKEN_ELSE)) {
        snprintf(line, sizeof(line), "goto %s", end_label);
        add_code(line);
        snprintf(line, sizeof(line), "%s:", false_label);
        add_code(line);

        advance();
        parse_block(in_function);

        snprintf(line, sizeof(line), "%s:", end_label);
        add_code(line);
    } else {
        snprintf(line, sizeof(line), "%s:", false_label);
        add_code(line);
    }
}

static void parse_while_statement(int in_function) {
    advance();

    char start_label[16];
    char end_label[16];
    strcpy(start_label, generate_label());
    strcpy(end_label, generate_label());

    char line[MAX_LINE_LENGTH];
    snprintf(line, sizeof(line), "%s:", start_label);
    add_code(line);

    expect(TOKEN_LPAREN, "Expected '(' after 'while'");
    char *condition = parse_condition();
    expect(TOKEN_RPAREN, "Expected ')' after while condition");

    snprintf(line, sizeof(line), "ifFalse %s goto %s", condition, end_label);
    add_code(line);

    parse_block(in_function);

    snprintf(line, sizeof(line), "goto %s", start_label);
    add_code(line);
    snprintf(line, sizeof(line), "%s:", end_label);
    add_code(line);
}

static void parse_return_stmt(int in_function) {
    if (!in_function) {
        fprintf(stderr, "Syntax Error at line %d: 'return' only valid inside function\n",
                current_token()->line_number);
        parse_errors++;
    }

    advance();
    char *expr = parse_expression();

    char line[MAX_LINE_LENGTH];
    snprintf(line, sizeof(line), "return %s", expr);
    add_code(line);

    expect(TOKEN_SEMICOLON, "Expected ';' after return statement");
}

static void parse_function_declaration() {
    advance();

    if (!match(TOKEN_IDENTIFIER)) {
        fprintf(stderr, "Syntax Error at line %d: Expected function name\n",
                current_token()->line_number);
        parse_errors++;
        return;
    }

    char function_name[MAX_LEXEME_LENGTH];
    strcpy(function_name, current_token()->lexeme);
    advance();

    expect(TOKEN_LPAREN, "Expected '(' after function name");

    if (!match(TOKEN_RPAREN)) {
        while (1) {
            if (!is_type_token(current_token()->type)) {
                fprintf(stderr, "Syntax Error at line %d: Expected parameter type\n",
                        current_token()->line_number);
                parse_errors++;
                break;
            }

            const char *param_type = token_type_to_datatype(current_token()->type);
            advance();

            if (!match(TOKEN_IDENTIFIER)) {
                fprintf(stderr, "Syntax Error at line %d: Expected parameter name\n",
                        current_token()->line_number);
                parse_errors++;
                break;
            }

            if (add_symbol(current_token()->lexeme, param_type) == -1) {
                parse_errors++;
            } else {
                mark_initialized(current_token()->lexeme);
            }
            advance();

            if (match(TOKEN_COMMA)) {
                advance();
            } else {
                break;
            }
        }
    }

    expect(TOKEN_RPAREN, "Expected ')' after parameters");

    char line[MAX_LINE_LENGTH];
    snprintf(line, sizeof(line), "func %s:", function_name);
    add_code(line);

    parse_block(1);

    snprintf(line, sizeof(line), "endfunc %s", function_name);
    add_code(line);
}

static void parse_statement(int in_function) {
    if (is_type_token(current_token()->type) && token_at(1)->type == TOKEN_IDENTIFIER && token_at(2)->type == TOKEN_LPAREN) {
        parse_function_declaration();
    } else if (is_type_token(current_token()->type)) {
        parse_declaration();
    } else if (match(TOKEN_PRINT)) {
        parse_print_stmt();
    } else if (match(TOKEN_IF)) {
        parse_if_statement(in_function);
    } else if (match(TOKEN_WHILE)) {
        parse_while_statement(in_function);
    } else if (match(TOKEN_RETURN)) {
        parse_return_stmt(in_function);
    } else if (match(TOKEN_IDENTIFIER)) {
        parse_assignment_or_call();
    } else if (match(TOKEN_EOF) || match(TOKEN_RBRACE)) {
        return;
    } else {
        fprintf(stderr, "Syntax Error at line %d: Unexpected token '%s'\n",
                current_token()->line_number, current_token()->lexeme);
        parse_errors++;
        advance();
    }
}

static void parse_statement_list(int stop_on_rbrace, int in_function) {
    while (!match(TOKEN_EOF) && !(stop_on_rbrace && match(TOKEN_RBRACE))) {
        parse_statement(in_function);
    }
}

static void parse_program() {
    parse_statement_list(0, 0);
}

int parse(TokenList *token_list) {
    tokens = token_list;
    current_token_index = 0;
    parse_errors = 0;

    printf("\n========== PARSING ==========\n");
    parse_program();

    if (parse_errors == 0) {
        printf("Parsing completed successfully!\n");
        printf("=============================\n");
        return 1;
    }

    printf("Parsing failed with %d error(s).\n", parse_errors);
    printf("=============================\n");
    return 0;
}

void print_parse_result(int success) {
    printf("\n========== SYNTAX VALIDATION ==========\n");
    if (success) {
        printf("Status: SUCCESS\n");
        printf("All statements are syntactically correct.\n");
    } else {
        printf("Status: FAILED\n");
        printf("Please fix the syntax errors and try again.\n");
    }
    printf("=======================================\n");
}
