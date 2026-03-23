#ifndef INTERMEDIATE_CODE_H
#define INTERMEDIATE_CODE_H

#define MAX_CODE_LENGTH 1000
#define MAX_LINE_LENGTH 256

typedef struct {
    char code[MAX_CODE_LENGTH][MAX_LINE_LENGTH];
    int count;
    int temp_count;
    int label_count;
} IntermediateCode;

// Function prototypes
void init_intermediate_code();
void add_code(const char *code_line);
char* generate_temp();
char* generate_label();
void optimize_intermediate_code();
void print_intermediate_code();
void print_bytecode();

#endif
