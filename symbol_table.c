#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

static SymbolTable symbol_table;

// Initialize symbol table
void init_symbol_table() {
    symbol_table.count = 0;
}

// Add a symbol to the table
int add_symbol(const char *name, const char *datatype) {
    // Check if symbol already exists
    if (lookup_symbol(name) != -1) {
        fprintf(stderr, "Error: Variable '%s' already declared\n", name);
        return -1;
    }
    
    // Check if table is full
    if (symbol_table.count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: Symbol table is full\n");
        return -1;
    }
    
    // Add new symbol
    SymbolEntry *entry = &symbol_table.entries[symbol_table.count];
    strncpy(entry->name, name, MAX_NAME_LENGTH - 1);
    entry->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(entry->datatype, datatype, 19);
    entry->datatype[19] = '\0';
    entry->value = 0;
    entry->initialized = 0;
    
    symbol_table.count++;
    return symbol_table.count - 1;
}

// Look up a symbol by name
int lookup_symbol(const char *name) {
    for (int i = 0; i < symbol_table.count; i++) {
        if (strcmp(symbol_table.entries[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Update symbol value
void update_symbol_value(const char *name, int value) {
    int index = lookup_symbol(name);
    if (index != -1) {
        symbol_table.entries[index].value = value;
    }
}

// Mark symbol as initialized
void mark_initialized(const char *name) {
    int index = lookup_symbol(name);
    if (index != -1) {
        symbol_table.entries[index].initialized = 1;
    }
}

// Check if symbol is initialized
int is_initialized(const char *name) {
    int index = lookup_symbol(name);
    if (index != -1) {
        return symbol_table.entries[index].initialized;
    }
    return 0;
}

// Print symbol table
void print_symbol_table() {
    printf("\n========== SYMBOL TABLE ==========\n");
    printf("%-15s %-15s %-15s %-15s\n", "Name", "Data Type", "Value", "Initialized");
    printf("--------------------------------------------------------------\n");
    
    for (int i = 0; i < symbol_table.count; i++) {
        SymbolEntry *entry = &symbol_table.entries[i];
        printf("%-15s %-15s %-15d %-15s\n",
               entry->name,
               entry->datatype,
               entry->value,
               entry->initialized ? "Yes" : "No");
    }
    printf("==============================================================\n");
}
