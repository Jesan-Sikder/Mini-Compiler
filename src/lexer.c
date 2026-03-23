#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static char *source_code = NULL;
static int position = 0;
static int line_number = 1;

// Initialize lexer by reading source file
void init_lexer(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        exit(1);
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory and read file
    source_code = (char*)malloc(file_size + 1);
    fread(source_code, 1, file_size, file);
    source_code[file_size] = '\0';
    
    fclose(file);
    position = 0;
    line_number = 1;
}

// Skip whitespace and comments
static void skip_whitespace() {
    while (source_code[position] != '\0') {
        if (source_code[position] == ' ' || source_code[position] == '\t' || source_code[position] == '\r') {
            position++;
        } else if (source_code[position] == '\n') {
            line_number++;
            position++;
        } else {
            break;
        }
    }
}

// Check if character is valid for identifier
static int is_valid_identifier_char(char c, int first) {
    if (first) {
        return isalpha(c) || c == '_';
    }
    return isalnum(c) || c == '_';
}

// Get next token from source code
static Token get_next_token() {
    Token token;
    token.line_number = line_number;
    memset(token.lexeme, 0, MAX_LEXEME_LENGTH);
    
    skip_whitespace();
    
    // End of file
    if (source_code[position] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }
    
    // Numbers (int and float)
    if (isdigit(source_code[position])) {
        int i = 0;
        int has_dot = 0;
        while (i < MAX_LEXEME_LENGTH - 1) {
            if (isdigit(source_code[position])) {
                token.lexeme[i++] = source_code[position++];
            } else if (source_code[position] == '.' && !has_dot) {
                has_dot = 1;
                token.lexeme[i++] = source_code[position++];
            } else {
                break;
            }
        }
        if (token.lexeme[i - 1] == '.') {
            position--;
            i--;
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_NUMBER;
        return token;
    }
    
    // Keywords and identifiers
    if (is_valid_identifier_char(source_code[position], 1)) {
        int i = 0;
        while (is_valid_identifier_char(source_code[position], 0) && i < MAX_LEXEME_LENGTH - 1) {
            token.lexeme[i++] = source_code[position++];
        }
        token.lexeme[i] = '\0';
        
        // Check for keywords
        if (strcmp(token.lexeme, "int") == 0) {
            token.type = TOKEN_INT;
        } else if (strcmp(token.lexeme, "float") == 0) {
            token.type = TOKEN_FLOAT;
        } else if (strcmp(token.lexeme, "print") == 0) {
            token.type = TOKEN_PRINT;
        } else if (strcmp(token.lexeme, "if") == 0) {
            token.type = TOKEN_IF;
        } else if (strcmp(token.lexeme, "else") == 0) {
            token.type = TOKEN_ELSE;
        } else if (strcmp(token.lexeme, "while") == 0) {
            token.type = TOKEN_WHILE;
        } else if (strcmp(token.lexeme, "return") == 0) {
            token.type = TOKEN_RETURN;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    // Multi-character operators
    if (source_code[position] == '=' && source_code[position + 1] == '=') {
        position += 2;
        token.type = TOKEN_EQUAL;
        strcpy(token.lexeme, "==");
        return token;
    }
    if (source_code[position] == '!' && source_code[position + 1] == '=') {
        position += 2;
        token.type = TOKEN_NOT_EQUAL;
        strcpy(token.lexeme, "!=");
        return token;
    }
    if (source_code[position] == '<' && source_code[position + 1] == '=') {
        position += 2;
        token.type = TOKEN_LESS_EQUAL;
        strcpy(token.lexeme, "<=");
        return token;
    }
    if (source_code[position] == '>' && source_code[position + 1] == '=') {
        position += 2;
        token.type = TOKEN_GREATER_EQUAL;
        strcpy(token.lexeme, ">=");
        return token;
    }
    
    // Single character tokens
    char current = source_code[position++];
    token.lexeme[0] = current;
    token.lexeme[1] = '\0';
    
    switch (current) {
        case '=':
            token.type = TOKEN_ASSIGN;
            break;
        case '<':
            token.type = TOKEN_LESS;
            break;
        case '>':
            token.type = TOKEN_GREATER;
            break;
        case '+':
            token.type = TOKEN_PLUS;
            break;
        case '-':
            token.type = TOKEN_MINUS;
            break;
        case '*':
            token.type = TOKEN_MULTIPLY;
            break;
        case '/':
            token.type = TOKEN_DIVIDE;
            break;
        case ';':
            token.type = TOKEN_SEMICOLON;
            break;
        case ',':
            token.type = TOKEN_COMMA;
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            break;
        case '{':
            token.type = TOKEN_LBRACE;
            break;
        case '}':
            token.type = TOKEN_RBRACE;
            break;
        case '[':
            token.type = TOKEN_LBRACKET;
            break;
        case ']':
            token.type = TOKEN_RBRACKET;
            break;
        default:
            token.type = TOKEN_INVALID;
            fprintf(stderr, "Error: Invalid token '%c' at line %d\n", current, line_number);
    }
    
    return token;
}

// Tokenize entire source code
TokenList tokenize() {
    TokenList token_list;
    token_list.count = 0;
    
    Token token;
    do {
        token = get_next_token();
        if (token_list.count < MAX_TOKENS) {
            token_list.tokens[token_list.count++] = token;
        } else {
            fprintf(stderr, "Error: Too many tokens\n");
            exit(1);
        }
    } while (token.type != TOKEN_EOF);
    
    return token_list;
}

// Get token type name for display
const char* get_token_name(TokenType type) {
    switch (type) {
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_PRINT: return "PRINT";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_LESS: return "LESS";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

// Print all tokens
void print_tokens(TokenList *token_list) {
    printf("\n========== TOKEN LIST ==========\n");
    printf("%-5s %-15s %-20s %-10s\n", "No.", "Token Type", "Lexeme", "Line");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < token_list->count; i++) {
        Token *token = &token_list->tokens[i];
        printf("%-5d %-15s %-20s %-10d\n", 
               i + 1, 
               get_token_name(token->type), 
               token->lexeme, 
               token->line_number);
    }
    printf("==================================================\n");
}

// Cleanup lexer and free allocated memory
void cleanup_lexer() {
    if (source_code != NULL) {
        free(source_code);
        source_code = NULL;
    }
}
