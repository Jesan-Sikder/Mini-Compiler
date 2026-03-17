#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SYMBOLS 100
#define MAX_NAME_LENGTH 100

typedef struct {
    char name[MAX_NAME_LENGTH];
    char datatype[20];
    int value;
    int initialized;
} SymbolEntry;

typedef struct {
    SymbolEntry entries[MAX_SYMBOLS];
    int count;
} SymbolTable;

// Function prototypes
void init_symbol_table();
int add_symbol(const char *name, const char *datatype);
int lookup_symbol(const char *name);
void update_symbol_value(const char *name, int value);
void mark_initialized(const char *name);
int is_initialized(const char *name);
void print_symbol_table();

#endif
