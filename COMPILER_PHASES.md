# COMPILER PHASES EXPLAINED

This document provides a detailed explanation of each compiler phase implemented in this Mini Compiler project. It is designed to help students understand how compilers work.

---

## TABLE OF CONTENTS
1. [Overview](#overview)
2. [Phase 1: Lexical Analysis](#phase-1-lexical-analysis)
3. [Phase 2: Syntax Analysis (Parsing)](#phase-2-syntax-analysis-parsing)
4. [Phase 3: Symbol Table Management](#phase-3-symbol-table-management)
5. [Phase 4: Intermediate Code Generation](#phase-4-intermediate-code-generation)
6. [Error Handling](#error-handling)
7. [Complete Example Walkthrough](#complete-example-walkthrough)

---

## OVERVIEW

A compiler translates source code from a high-level programming language to a lower-level language (like assembly or machine code). This mini compiler demonstrates the fundamental phases:

```
Source Code → Lexical Analysis → Syntax Analysis → Semantic Analysis → Intermediate Code Generation
```

Our compiler implements a **simplified pipeline** for an educational custom language.

---

## PHASE 1: LEXICAL ANALYSIS

### Purpose
Convert source code (sequence of characters) into a sequence of **tokens** (meaningful units).

### Implementation
- **File**: `lexer.c` and `lexer.h`
- **Main Function**: `tokenize()`

### How It Works

1. **Reading Source Code**
   - The lexer reads the entire source file into memory
   - Maintains current position and line number

2. **Token Recognition**
   The lexer recognizes these token types:
   - **Keywords**: `int`, `print`
   - **Identifiers**: Variable names (e.g., `a`, `sum`, `count`)
   - **Numbers**: Integer literals (e.g., `10`, `42`)
   - **Operators**: `+`, `-`, `*`, `/`, `=`
   - **Delimiters**: `;`, `(`, `)`

3. **Tokenization Process**
   ```
   Input:  int a = 10;
   
   Output: [INT] [IDENTIFIER:a] [ASSIGN:=] [NUMBER:10] [SEMICOLON:;]
   ```

4. **Whitespace Handling**
   - Skips spaces, tabs, and newlines
   - Tracks line numbers for error reporting

### Example Code Flow

```c
// Input
int x = 5;

// Lexer identifies:
Token 1: type=TOKEN_INT,        lexeme="int",  line=1
Token 2: type=TOKEN_IDENTIFIER, lexeme="x",    line=1
Token 3: type=TOKEN_ASSIGN,     lexeme="=",    line=1
Token 4: type=TOKEN_NUMBER,     lexeme="5",    line=1
Token 5: type=TOKEN_SEMICOLON,  lexeme=";",    line=1
```

### Key Algorithms

**Identifier Recognition**:
```
1. Check if first character is letter or underscore
2. Continue while character is letter, digit, or underscore
3. Check if token is a keyword ("int", "print")
4. If not keyword, it's an IDENTIFIER
```

**Number Recognition**:
```
1. Check if character is digit
2. Continue while character is digit
3. Return NUMBER token
```

---

## PHASE 2: SYNTAX ANALYSIS (PARSING)

### Purpose
Verify that tokens form valid statements according to the **grammar rules**.

### Implementation
- **File**: `parser.c` and `parser.h`
- **Technique**: **Recursive Descent Parsing**

### Grammar Rules (BNF)

```bnf
program        → statement_list
statement_list → statement statement_list | statement
statement      → declaration | assignment | print_stmt

declaration    → int identifier ; | int identifier = number ;
assignment     → identifier = expression ;
print_stmt     → print ( identifier ) ;

expression     → term | expression + term | expression - term
term           → factor | term * factor | term / factor
factor         → identifier | number
```

### How It Works

1. **Recursive Descent**
   - Each grammar rule has a corresponding function
   - Functions call each other recursively based on grammar

2. **Top-Down Parsing**
   ```
   parse_program()
     └─> parse_statement_list()
           └─> parse_statement()
                 ├─> parse_declaration()
                 ├─> parse_assignment()
                 └─> parse_print_stmt()
   ```

3. **Expression Parsing**
   Handles operator precedence correctly:
   - `*` and `/` have higher precedence than `+` and `-`
   - Example: `a + b * 2` parsed as `a + (b * 2)`

### Example Parsing

**Input**: `int x = 5;`

```
1. parse_statement() recognizes TOKEN_INT
2. Calls parse_declaration()
3. Expects TOKEN_IDENTIFIER → found "x" ✓
4. Sees TOKEN_ASSIGN
5. Expects TOKEN_NUMBER → found "5" ✓
6. Expects TOKEN_SEMICOLON → found ";" ✓
7. Success!
```

**Input with error**: `int = 5;`

```
1. parse_statement() recognizes TOKEN_INT
2. Calls parse_declaration()
3. Expects TOKEN_IDENTIFIER → found "=" ✗
4. ERROR: "Expected identifier after 'int'"
```

### Operator Precedence

Implemented through grammar structure:
- **expression** handles `+` and `-` (lower precedence)
- **term** handles `*` and `/` (higher precedence)
- **factor** handles basic elements (identifiers, numbers)

---

## PHASE 3: SYMBOL TABLE MANAGEMENT

### Purpose
Keep track of all variables: their names, types, values, and initialization status.

### Implementation
- **File**: `symbol_table.c` and `symbol_table.h`
- **Data Structure**: Array of symbol entries

### Symbol Entry Structure

```c
typedef struct {
    char name[100];       // Variable name
    char datatype[20];    // Data type ("int")
    int value;            // Current value
    int initialized;      // Has it been assigned?
} SymbolEntry;
```

### Key Operations

1. **add_symbol(name, type)**
   - Adds a new variable to the table
   - Checks for duplicate declarations
   - Returns error if variable already exists

2. **lookup_symbol(name)**
   - Searches for a variable by name
   - Returns index if found, -1 if not found
   - Used to check if variable is declared before use

3. **mark_initialized(name)**
   - Marks a variable as initialized
   - Called during declaration with initialization or assignment

4. **update_symbol_value(name, value)**
   - Updates the value of a variable

### Example Flow

```c
// Input code
int a;          // Add symbol: a, type=int, initialized=0
int b = 10;     // Add symbol: b, type=int, initialized=1, value=10
a = b + 5;      // Mark 'a' as initialized

// Symbol Table State:
// Name   Type   Value   Initialized
// a      int    0       Yes
// b      int    10      Yes
```

### Error Detection

1. **Undeclared Variable**
   ```c
   x = 5;  // Error: 'x' not declared
   ```
   Parser calls `lookup_symbol("x")` → returns -1 → Error!

2. **Duplicate Declaration**
   ```c
   int a;
   int a;  // Error: 'a' already declared
   ```
   `add_symbol("a")` checks if exists → Error!

---

## PHASE 4: INTERMEDIATE CODE GENERATION

### Purpose
Generate **Three-Address Code (TAC)**, a low-level representation easier to optimize and translate to machine code.

### Implementation
- **File**: `intermediate_code.c` and `intermediate_code.h`

### Three-Address Code Format

Each instruction has **at most three addresses** (operands):

```
result = operand1 operator operand2
```

Examples:
- `t0 = a + b`
- `x = t0`
- `print x`

### How It Works

1. **Temporary Variables**
   - Generated for complex expressions
   - Named `t0`, `t1`, `t2`, etc.
   - Function: `generate_temp()`

2. **Expression Translation**
   
   **Example 1**: `a = b + 5;`
   ```
   t0 = b + 5
   a = t0
   ```

   **Example 2**: `c = a + b * 2;`
   ```
   t0 = b * 2      // Higher precedence first
   t1 = a + t0
   c = t1
   ```

   **Example 3**: `x = (a + b) * (c - d);`
   ```
   t0 = a + b
   t1 = c - d
   t2 = t0 * t1
   x = t2
   ```

3. **Code Generation During Parsing**
   - As the parser validates expressions, it generates TAC
   - Each arithmetic operation creates a TAC instruction
   - Temporary variables store intermediate results

### Advantages of Three-Address Code

1. **Simplicity**: Each instruction is simple and atomic
2. **Optimization**: Easy to apply optimization techniques
3. **Translation**: Easy to convert to assembly or machine code
4. **Portability**: Independent of target architecture

### Example Walkthrough

**Source Code**:
```c
int a = 10;
int b = 20;
int c;
c = a + b * 2;
```

**Generated TAC**:
```
1: a = 10
2: b = 20
3: t0 = b * 2      // Multiply first (higher precedence)
4: t1 = a + t0     // Then add
5: c = t1          // Final assignment
```

---

## ERROR HANDLING

### Types of Errors Detected

1. **Lexical Errors**
   - Invalid characters
   - Example: `int a = @10;` → Invalid token '@'

2. **Syntax Errors**
   - Missing semicolon
   - Unexpected token
   - Example: `int = 5;` → Expected identifier

3. **Semantic Errors**
   - Undeclared variable usage
   - Duplicate variable declaration
   - Example: `x = 5;` (if x not declared)

### Error Reporting

Errors include:
- **Error type**: Syntax Error, Semantic Error
- **Line number**: Where the error occurred
- **Description**: What went wrong
- **Context**: Expected vs. found

**Example Error Messages**:
```
Error at line 3: Undeclared variable 'x'
Syntax Error at line 2: Expected ';' at end of statement
Error: Variable 'a' already declared
```

---

## COMPLETE EXAMPLE WALKTHROUGH

Let's trace the complete compilation of a simple program:

### Input Program (program.txt)
```c
int a;
int b = 10;
a = b + 5;
print(a);
```

### PHASE 1: Lexical Analysis

**Tokens Generated**:
```
1.  [INT]        "int"      line 1
2.  [IDENTIFIER] "a"        line 1
3.  [SEMICOLON]  ";"        line 1
4.  [INT]        "int"      line 2
5.  [IDENTIFIER] "b"        line 2
6.  [ASSIGN]     "="        line 2
7.  [NUMBER]     "10"       line 2
8.  [SEMICOLON]  ";"        line 2
9.  [IDENTIFIER] "a"        line 3
10. [ASSIGN]     "="        line 3
11. [IDENTIFIER] "b"        line 3
12. [PLUS]       "+"        line 3
13. [NUMBER]     "5"        line 3
14. [SEMICOLON]  ";"        line 3
15. [PRINT]      "print"    line 4
16. [LPAREN]     "("        line 4
17. [IDENTIFIER] "a"        line 4
18. [RPAREN]     ")"        line 4
19. [SEMICOLON]  ";"        line 4
20. [EOF]        "EOF"      line 4
```

### PHASE 2: Syntax Analysis

**Parse Tree** (conceptual):
```
program
  └─ statement_list
       ├─ statement (declaration: int a;)
       ├─ statement (declaration: int b = 10;)
       ├─ statement (assignment: a = b + 5;)
       └─ statement (print: print(a);)
```

**Parsing Actions**:
```
1. Parse declaration: "int a;"
   - Add 'a' to symbol table
   
2. Parse declaration: "int b = 10;"
   - Add 'b' to symbol table
   - Mark 'b' as initialized
   - Generate TAC: "b = 10"
   
3. Parse assignment: "a = b + 5;"
   - Check 'a' is declared ✓
   - Parse expression "b + 5"
   - Generate TAC: "t0 = b + 5"
   - Generate TAC: "a = t0"
   - Mark 'a' as initialized
   
4. Parse print: "print(a);"
   - Check 'a' is declared ✓
   - Generate TAC: "print a"
```

### PHASE 3: Symbol Table

**Final Symbol Table**:
```
Name    Type    Value    Initialized
a       int     0        Yes
b       int     10       Yes
```

### PHASE 4: Intermediate Code

**Generated Three-Address Code**:
```
1: b = 10
2: t0 = b + 5
3: a = t0
4: print a
```

### Understanding the Output

The intermediate code represents:
1. Initialize `b` with 10
2. Calculate `b + 5` and store in temporary `t0`
3. Assign `t0` to `a`
4. Print value of `a`

If executed, this would print: **15**

---

## CONCLUSION

This mini compiler demonstrates fundamental compiler concepts:

1. **Lexical Analysis** breaks code into tokens
2. **Syntax Analysis** validates structure using grammar
3. **Symbol Table** tracks variable information
4. **Intermediate Code** provides low-level representation
5. **Error Handling** detects and reports problems

Each phase builds upon the previous one, creating a complete compilation pipeline. This modular approach makes it easy to understand, extend, and maintain.

---

## FURTHER READING

To learn more about compiler design:

1. **Books**:
   - "Compilers: Principles, Techniques, and Tools" (Dragon Book)
   - "Modern Compiler Implementation in C"
   - "Engineering a Compiler"

2. **Topics to Explore**:
   - Optimization techniques
   - Code generation
   - Advanced parsing (LR parsing)
   - Type systems
   - Control flow analysis

3. **Extensions to Try**:
   - Add if-else statements
   - Implement while loops
   - Support for functions
   - Array support
   - Float data type
   - Optimization passes

---

**Happy Learning!** 🎓
