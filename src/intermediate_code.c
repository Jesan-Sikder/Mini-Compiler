#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "intermediate_code.h"

static IntermediateCode ic;

// Initialize intermediate code generator
void init_intermediate_code() {
    ic.count = 0;
    ic.temp_count = 0;
    ic.label_count = 0;
}

// Add a line of intermediate code
void add_code(const char *code_line) {
    if (ic.count < MAX_CODE_LENGTH) {
        strncpy(ic.code[ic.count], code_line, MAX_LINE_LENGTH - 1);
        ic.code[ic.count][MAX_LINE_LENGTH - 1] = '\0';
        ic.count++;
    } else {
        fprintf(stderr, "Error: Intermediate code buffer is full\n");
    }
}

// Generate a new temporary variable
/* NOTE: Returns pointer to static buffer. This is acceptable for this
 * educational compiler because the result is immediately used in sprintf()
 * calls before the next temp is generated. For production code, consider
 * returning a dynamically allocated string or using a different approach.
 */
char* generate_temp() {
    static char temp[10];
    sprintf(temp, "t%d", ic.temp_count++);
    return temp;
}

char* generate_label() {
    static char label[16];
    snprintf(label, sizeof(label), "L%d", ic.label_count++);
    return label;
}

static int is_numeric_literal(const char *s) {
    if (s == NULL || *s == '\0') {
        return 0;
    }
    char *endptr = NULL;
    strtod(s, &endptr);
    return endptr != s && *endptr == '\0';
}

void optimize_intermediate_code() {
    for (int i = 0; i < ic.count; i++) {
        char lhs[64], op1[64], op2[64];
        char op = '\0';
        if (sscanf(ic.code[i], "%63s = %63s %c %63s", lhs, op1, &op, op2) == 4) {
            if (is_numeric_literal(op1) && is_numeric_literal(op2) &&
                (op == '+' || op == '-' || op == '*' || op == '/')) {
                double a = atof(op1);
                double b = atof(op2);
                double r = 0.0;
                int can_optimize = 1;
                if (op == '+') r = a + b;
                else if (op == '-') r = a - b;
                else if (op == '*') r = a * b;
                else if (op == '/') {
                    if (b == 0.0) {
                        can_optimize = 0;
                    } else {
                        r = a / b;
                    }
                }
                if (can_optimize) {
                    snprintf(ic.code[i], MAX_LINE_LENGTH, "%s = %g", lhs, r);
                }
            }
        }
    }
}

// Print intermediate code
void print_intermediate_code() {
    printf("\n========== INTERMEDIATE CODE (Three Address Code) ==========\n");
    
    if (ic.count == 0) {
        printf("No intermediate code generated.\n");
    } else {
        for (int i = 0; i < ic.count; i++) {
            printf("%3d: %s\n", i + 1, ic.code[i]);
        }
    }
    
    printf("============================================================\n");
}

void print_bytecode() {
    printf("\n========== BYTECODE (Stack-style) ==========\n");
    if (ic.count == 0) {
        printf("No bytecode generated.\n");
    } else {
        for (int i = 0; i < ic.count; i++) {
            char a[64], b[64], c[64], d[64];
            if (sscanf(ic.code[i], "%63s = %63s %63s %63s", a, b, c, d) == 4 &&
                (strcmp(c, "+") == 0 || strcmp(c, "-") == 0 || strcmp(c, "*") == 0 || strcmp(c, "/") == 0)) {
                printf("%3d: PUSH %s | PUSH %s | %s | STORE %s\n",
                       i + 1, b, d,
                       strcmp(c, "+") == 0 ? "ADD" :
                       strcmp(c, "-") == 0 ? "SUB" :
                       strcmp(c, "*") == 0 ? "MUL" : "DIV",
                       a);
            } else if (sscanf(ic.code[i], "%63s = %63s", a, b) == 2) {
                printf("%3d: LOAD %s | STORE %s\n", i + 1, b, a);
            } else if (strncmp(ic.code[i], "print ", 6) == 0) {
                printf("%3d: LOAD %s | PRINT\n", i + 1, ic.code[i] + 6);
            } else if (strncmp(ic.code[i], "ifFalse ", 8) == 0) {
                printf("%3d: JZ %s\n", i + 1, ic.code[i] + 8);
            } else if (strncmp(ic.code[i], "goto ", 5) == 0) {
                printf("%3d: JMP %s\n", i + 1, ic.code[i] + 5);
            } else {
                printf("%3d: %s\n", i + 1, ic.code[i]);
            }
        }
    }
    printf("============================================\n");
}
