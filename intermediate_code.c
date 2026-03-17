#include <stdio.h>
#include <string.h>
#include "intermediate_code.h"

static IntermediateCode ic;

// Initialize intermediate code generator
void init_intermediate_code() {
    ic.count = 0;
    ic.temp_count = 0;
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
