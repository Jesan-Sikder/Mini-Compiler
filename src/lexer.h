#ifndef LEXER_H
#define LEXER_H

#define MAX_LEXEME_LENGTH 100
#define MAX_TOKENS 1000

typedef enum {
    TOKEN_INT,          // int keyword
    TOKEN_FLOAT,        // float keyword
    TOKEN_PRINT,        // print keyword
    TOKEN_IF,           // if keyword
    TOKEN_ELSE,         // else keyword
    TOKEN_WHILE,        // while keyword
    TOKEN_RETURN,       // return keyword
    TOKEN_IDENTIFIER,   // variable names
    TOKEN_NUMBER,       // numeric literals
    TOKEN_ASSIGN,       // =
    TOKEN_EQUAL,        // ==
    TOKEN_NOT_EQUAL,    // !=
    TOKEN_LESS,         // <
    TOKEN_GREATER,      // >
    TOKEN_LESS_EQUAL,   // <=
    TOKEN_GREATER_EQUAL,// >=
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_MULTIPLY,     // *
    TOKEN_DIVIDE,       // /
    TOKEN_SEMICOLON,    // ;
    TOKEN_COMMA,        // ,
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    TOKEN_LBRACKET,     // [
    TOKEN_RBRACKET,     // ]
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
