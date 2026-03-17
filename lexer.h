#ifndef LEXER_H
#define LEXER_H

#define MAX_LEXEME_LENGTH 100
#define MAX_TOKENS 1000

typedef enum {
    TOKEN_INT,          // int keyword
    TOKEN_PRINT,        // print keyword
    TOKEN_IDENTIFIER,   // variable names
    TOKEN_NUMBER,       // integer literals
    TOKEN_ASSIGN,       // =
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_MULTIPLY,     // *
    TOKEN_DIVIDE,       // /
    TOKEN_SEMICOLON,    // ;
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_EOF,          // End of file
    TOKEN_INVALID       // Invalid token
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME_LENGTH];
    int line_number;
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
} TokenList;

// Function prototypes
void init_lexer(const char *filename);
TokenList tokenize();
void print_tokens(TokenList *token_list);
const char* get_token_name(TokenType type);
void cleanup_lexer();

#endif
