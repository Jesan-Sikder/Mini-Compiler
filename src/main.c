#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"
#include "intermediate_code.h"


void print_banner() {
    printf("\n");
    printf("|-----------------------------------------------------------|\n");
    printf("|                                                           |\n");
    printf("|        MINI COMPILER - Compiler Design Project            |\n");
    printf("|                                                           |\n");
    printf("|  A mini compiler with structured language extensions      |\n");
    printf("|  Implements: Lexical Analysis, Syntax Analysis,           |\n");
    printf("|  Symbol Table,  Intermediate Code Generation,             |\n");
    printf("|       Optimization and Code Generation .....              |\n");
    printf("|-----------------------------------------------------------|\n");
    printf("\n");
}


void print_usage() {
    printf("Usage: compiler <input_file>\n");
    printf("Example: compiler program.txt\n");
}

int main(int argc, char *argv[]) {
    print_banner();
    
    // Check command line arguments
    if (argc != 2) {
        fprintf(stderr, "Error: Invalid number of arguments.\n\n");
        print_usage();
        return 1;
    }
    
    const char *input_file = argv[1];
    printf("Compiling file: %s\n", input_file);
    printf("|-----------------------------------------------------------|\n");
    
    // Phase 1: Lexical Analysis
    printf("\nPhase 1: Lexical Analysis\n");
    printf("|-----------------------------------------------------------|\n");
    init_lexer(input_file);
    TokenList token_list = tokenize();
    print_tokens(&token_list);
    
    // Phase 2: Initialize Symbol Table and Intermediate Code Generator
    init_symbol_table();
    init_intermediate_code();
    
    // Phase 3: Syntax Analysis (Parsing)
    printf("\nPhase 2: Syntax Analysis\n");
    printf("|-----------------------------------------------------------|\n");
    int parse_success = parse(&token_list);
    print_parse_result(parse_success);
    
    // If parsing failed, don't continue
    if (!parse_success) {
        printf("\nCompilation terminated due to errors.\n");
        return 1;
    }
    
    // Phase 4: Display Symbol Table
    printf("\nPhase 3: Symbol Table\n");
    printf("|-----------------------------------------------------------|\n");
    print_symbol_table();
    
    // Phase 5: Display Intermediate Code
    printf("\nPhase 4: Intermediate Code Generation\n");
    printf("|-----------------------------------------------------------|\n");
    print_intermediate_code();

    // Phase 6: Optimization
    printf("\nPhase 5: Optimization\n");
    printf("|-----------------------------------------------------------|\n");
    optimize_intermediate_code();
    print_intermediate_code();

    // Phase 7: Code generation
    printf("\nPhase 6: Code Generation (Bytecode)\n");
    printf("|-----------------------------------------------------------|\n");
    print_bytecode();
    
    // Compilation successful
    printf("\n|-----------------------------------------------------------|\n");
    printf("|                                                           |\n");
    printf("|           COMPILATION COMPLETED SUCCESSFULLY!             |\n");
    printf("|                                                           |\n");
    printf("|-----------------------------------------------------------|\n");
    printf("\n");
    
    // Cleanup
    cleanup_lexer();
    
    return 0;
}
