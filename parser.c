#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "symbol_table.h"
#include "intermediate_code.h"

static TokenList *tokens;
static int current_token_index;
static int parse_errors;

// Helper functions
static Token* current_token() {
    if (current_token_index < tokens->count) {
        return &tokens->tokens[current_token_index];
    }
    return &tokens->tokens[tokens->count - 1]; // Return EOF
}

static void advance() {
    if (current_token_index < tokens->count - 1) {
        current_token_index++;
    }
}

static int match(TokenType type) {
    return current_token()->type == type;
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

// Forward declarations for recursive descent parser
static char* parse_expression();
static char* parse_term();
static char* parse_factor();

/* NOTE ON STATIC VARIABLES IN PARSING FUNCTIONS:
 * This educational compiler uses static buffers to return string results from
 * parse_expression(), parse_term(), and parse_factor(). While this approach has
 * limitations (the buffers are shared across invocations), it works correctly
 * for this simple grammar because:
 * 1. Results are immediately copied/used by callers before the next invocation
 * 2. The grammar is simple with left-to-right evaluation
 * 3. This keeps the code simple and readable for students
 * 
 * For production compilers, consider using:
 * - Dynamic memory allocation with proper cleanup
 * - Caller-provided buffers
 * - Abstract Syntax Tree (AST) instead of string-based results
 */

// Parse factor: identifier | number
static char* parse_factor() {
    static char result[MAX_LEXEME_LENGTH];
    
    if (match(TOKEN_IDENTIFIER)) {
        strcpy(result, current_token()->lexeme);
        
        // Check if variable is declared
        if (lookup_symbol(result) == -1) {
            fprintf(stderr, "Error at line %d: Undeclared variable '%s'\n", 
                    current_token()->line_number, result);
            parse_errors++;
        }
        
        advance();
        return result;
    } else if (match(TOKEN_NUMBER)) {
        strcpy(result, current_token()->lexeme);
        advance();
        return result;
    } else {
        fprintf(stderr, "Syntax Error at line %d: Expected identifier or number\n", 
                current_token()->line_number);
        parse_errors++;
        strcpy(result, "0");
        return result;
    }
}

// Parse term: factor | term * factor | term / factor
static char* parse_term() {
    char* left = parse_factor();
    static char result[MAX_LEXEME_LENGTH];
    strcpy(result, left);
    
    while (match(TOKEN_MULTIPLY) || match(TOKEN_DIVIDE)) {
        char op = current_token()->lexeme[0];
        advance();
        
        char* right = parse_factor();
        
        // Generate intermediate code
        char* temp = generate_temp();
        char code_line[MAX_LINE_LENGTH];
        sprintf(code_line, "%s = %s %c %s", temp, result, op, right);
        add_code(code_line);
        
        strcpy(result, temp);
    }
    
    return result;
}

// Parse expression: term | expression + term | expression - term
static char* parse_expression() {
    char* left = parse_term();
    static char result[MAX_LEXEME_LENGTH];
    strcpy(result, left);
    
    while (match(TOKEN_PLUS) || match(TOKEN_MINUS)) {
        char op = current_token()->lexeme[0];
        advance();
        
        char* right = parse_term();
        
        // Generate intermediate code
        char* temp = generate_temp();
        char code_line[MAX_LINE_LENGTH];
        sprintf(code_line, "%s = %s %c %s", temp, result, op, right);
        add_code(code_line);
        
        strcpy(result, temp);
    }
    
    return result;
}

// Parse print statement: print ( identifier ) ;
static void parse_print_stmt() {
    advance(); // consume 'print'
    expect(TOKEN_LPAREN, "Expected '(' after 'print'");
    
    if (match(TOKEN_IDENTIFIER)) {
        char var_name[MAX_LEXEME_LENGTH];
        strcpy(var_name, current_token()->lexeme);
        
        // Check if variable is declared
        if (lookup_symbol(var_name) == -1) {
            fprintf(stderr, "Error at line %d: Undeclared variable '%s'\n", 
                    current_token()->line_number, var_name);
            parse_errors++;
        }
        
        advance();
        
        // Generate intermediate code for print
        char code_line[MAX_LINE_LENGTH];
        sprintf(code_line, "print %s", var_name);
        add_code(code_line);
    } else {
        fprintf(stderr, "Syntax Error at line %d: Expected identifier in print statement\n",
                current_token()->line_number);
        parse_errors++;
    }
    
    expect(TOKEN_RPAREN, "Expected ')' after identifier");
    expect(TOKEN_SEMICOLON, "Expected ';' at end of statement");
}

// Parse assignment: identifier = expression ;
static void parse_assignment() {
    char var_name[MAX_LEXEME_LENGTH];
    strcpy(var_name, current_token()->lexeme);
    
    // Check if variable is declared
    if (lookup_symbol(var_name) == -1) {
        fprintf(stderr, "Error at line %d: Undeclared variable '%s'\n", 
                current_token()->line_number, var_name);
        parse_errors++;
    } else {
        mark_initialized(var_name);
    }
    
    advance();
    expect(TOKEN_ASSIGN, "Expected '=' in assignment");
    
    char* expr_result = parse_expression();
    
    // Generate intermediate code for assignment
    char code_line[MAX_LINE_LENGTH];
    sprintf(code_line, "%s = %s", var_name, expr_result);
    add_code(code_line);
    
    expect(TOKEN_SEMICOLON, "Expected ';' at end of statement");
}

// Parse declaration: int identifier ; | int identifier = number ;
static void parse_declaration() {
    advance(); // consume 'int'
    
    if (!match(TOKEN_IDENTIFIER)) {
        fprintf(stderr, "Syntax Error at line %d: Expected identifier after 'int'\n",
                current_token()->line_number);
        parse_errors++;
        return;
    }
    
    char var_name[MAX_LEXEME_LENGTH];
    strcpy(var_name, current_token()->lexeme);
    
    // Add to symbol table
    if (add_symbol(var_name, "int") == -1) {
        parse_errors++;
    }
    
    advance();
    
    // Check for initialization
    if (match(TOKEN_ASSIGN)) {
        advance();
        
        if (!match(TOKEN_NUMBER)) {
            fprintf(stderr, "Syntax Error at line %d: Expected number after '='\n",
                    current_token()->line_number);
            parse_errors++;
        } else {
            int value = atoi(current_token()->lexeme);
            update_symbol_value(var_name, value);
            mark_initialized(var_name);
            
            // Generate intermediate code for initialization
            char code_line[MAX_LINE_LENGTH];
            sprintf(code_line, "%s = %s", var_name, current_token()->lexeme);
            add_code(code_line);
            
            advance();
        }
    }
    
    expect(TOKEN_SEMICOLON, "Expected ';' at end of declaration");
}

// Parse statement
static void parse_statement() {
    if (match(TOKEN_INT)) {
        parse_declaration();
    } else if (match(TOKEN_PRINT)) {
        parse_print_stmt();
    } else if (match(TOKEN_IDENTIFIER)) {
        parse_assignment();
    } else if (match(TOKEN_EOF)) {
        // End of program
        return;
    } else {
        fprintf(stderr, "Syntax Error at line %d: Unexpected token '%s'\n",
                current_token()->line_number, current_token()->lexeme);
        parse_errors++;
        advance(); // Skip the error token
    }
}

// Parse statement list
static void parse_statement_list() {
    while (!match(TOKEN_EOF)) {
        parse_statement();
    }
}

// Parse program: statement_list
static void parse_program() {
    parse_statement_list();
}

// Main parse function
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
    } else {
        printf("Parsing failed with %d error(s).\n", parse_errors);
        printf("=============================\n");
        return 0;
    }
}

// Print parse result
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
