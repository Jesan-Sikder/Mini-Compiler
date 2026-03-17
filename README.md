# Mini Compiler - Compiler Design Project

## Project Title
**Design and Implementation of a Mini Compiler Using C**

## Overview
This is a complete mini compiler implementation in C for educational purposes. It demonstrates the fundamental phases of a compiler including lexical analysis, syntax analysis, symbol table management, and intermediate code generation.

## Features
- ✅ **Lexical Analysis**: Tokenizes source code into meaningful tokens
- ✅ **Syntax Analysis**: Validates grammar using recursive descent parsing
- ✅ **Symbol Table Management**: Tracks variable declarations and usage
- ✅ **Intermediate Code Generation**: Generates three-address code
- ✅ **Error Handling**: Detects and reports syntax and semantic errors

## Supported Language Features

### Data Type
- `int` (integer type only)

### Statements
1. **Variable Declaration**
   ```c
   int a;
   int b = 10;
   ```

2. **Assignment Statement**
   ```c
   a = b + 5;
   ```

3. **Arithmetic Expressions**
   - Operators: `+`, `-`, `*`, `/`
   - Example: `c = a + b * 2 - d / 3;`

4. **Print Statement**
   ```c
   print(a);
   ```

### Language Rules
- Only integer datatype is allowed
- One statement per line
- Each statement must end with semicolon (`;`)
- Variables must be declared before use
- Follows standard operator precedence (`*`, `/` before `+`, `-`)

## Formal Grammar

```
program        → statement_list
statement_list → statement statement_list | statement

statement      → declaration | assignment | print_stmt

declaration    → int identifier ;
               | int identifier = number ;

assignment     → identifier = expression ;

print_stmt     → print ( identifier ) ;

expression     → term
               | expression + term
               | expression - term

term           → factor
               | term * factor
               | term / factor

factor         → identifier | number
```

## Project Structure

```
Mini-compiler-/
├── main.c                  # Main program - integrates all modules
├── lexer.c                 # Lexical analyzer implementation
├── lexer.h                 # Lexer header file
├── parser.c                # Syntax analyzer implementation
├── parser.h                # Parser header file
├── symbol_table.c          # Symbol table implementation
├── symbol_table.h          # Symbol table header file
├── intermediate_code.c     # Intermediate code generator
├── intermediate_code.h     # Intermediate code header file
├── Makefile                # Build configuration
├── program.txt             # Sample input program
├── test1.txt               # Test case 1
├── test2.txt               # Test case 2
└── README.md               # This file
```

## Compilation and Execution

### Prerequisites
- GCC compiler (or any C compiler)
- Make utility (optional, for using Makefile)

### Building the Compiler

#### Using Make (Recommended)
```bash
make
```

#### Manual Compilation
```bash
gcc -Wall -Wextra -std=c99 -o compiler main.c lexer.c parser.c symbol_table.c intermediate_code.c
```

### Running the Compiler

#### Using Make
```bash
# Run with default program.txt
make run

# Run with test1.txt
make test1

# Run with test2.txt
make test2
```

#### Manual Execution
```bash
./compiler program.txt
./compiler test1.txt
./compiler test2.txt
```

### Other Make Commands
```bash
make clean    # Remove build artifacts
make rebuild  # Clean and rebuild
make help     # Show help message
```

## Sample Input and Output

### Sample Input (program.txt)
```c
int a;
int b = 10;
int c;
a = b + 5;
c = a * 2;
print(c);
```

### Sample Output
```
╔═══════════════════════════════════════════════════════════╗
║                                                           ║
║        MINI COMPILER - Compiler Design Project            ║
║                                                           ║
║  A simple compiler for educational purposes               ║
║  Implements: Lexical Analysis, Syntax Analysis,           ║
║  Symbol Table, and Intermediate Code Generation           ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝

Compiling file: program.txt
═══════════════════════════════════════════════════════════

Phase 1: Lexical Analysis
─────────────────────────────────────────────────────────

========== TOKEN LIST ==========
No.   Token Type      Lexeme               Line      
--------------------------------------------------
1     INT             int                  1         
2     IDENTIFIER      a                    1         
3     SEMICOLON       ;                    1         
4     INT             int                  2         
5     IDENTIFIER      b                    2         
6     ASSIGN          =                    2         
7     NUMBER          10                   2         
8     SEMICOLON       ;                    2         
...
==================================================

Phase 2: Syntax Analysis
─────────────────────────────────────────────────────────

========== PARSING ==========
Parsing completed successfully!
=============================

========== SYNTAX VALIDATION ==========
Status: SUCCESS
All statements are syntactically correct.
=======================================

Phase 3: Symbol Table
─────────────────────────────────────────────────────────

========== SYMBOL TABLE ==========
Name            Data Type       Value           Initialized    
--------------------------------------------------------------
a               int             0               Yes            
b               int             10              Yes            
c               int             0               Yes            
==============================================================

Phase 4: Intermediate Code Generation
─────────────────────────────────────────────────────────

========== INTERMEDIATE CODE (Three Address Code) ==========
  1: b = 10
  2: t0 = b + 5
  3: a = t0
  4: t1 = a * 2
  5: c = t1
  6: print c
============================================================

╔═══════════════════════════════════════════════════════════╗
║                                                           ║
║           COMPILATION COMPLETED SUCCESSFULLY!             ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
```

## Error Handling

The compiler detects and reports various errors:

1. **Undeclared Variable**
   ```c
   a = 5;  // Error: 'a' not declared
   ```

2. **Missing Semicolon**
   ```c
   int a  // Error: Expected ';'
   ```

3. **Invalid Token**
   ```c
   int a = @5;  // Error: Invalid token '@'
   ```

4. **Syntax Errors**
   ```c
   int = 5;  // Error: Expected identifier
   ```

## Module Descriptions

### 1. Lexical Analyzer (lexer.c/h)
- Reads source code and converts it into tokens
- Recognizes keywords, identifiers, numbers, and operators
- Tracks line numbers for error reporting

### 2. Parser (parser.c/h)
- Validates syntax using recursive descent parsing
- Implements the formal grammar
- Calls symbol table and intermediate code generation functions
- Reports syntax and semantic errors

### 3. Symbol Table (symbol_table.c/h)
- Maintains information about variables
- Tracks variable names, data types, values, and initialization status
- Supports variable lookup and updates

### 4. Intermediate Code Generator (intermediate_code.c/h)
- Generates three-address code
- Creates temporary variables for complex expressions
- Produces output suitable for further optimization or code generation

### 5. Main Program (main.c)
- Coordinates all compiler phases
- Displays formatted output
- Handles command-line arguments

## Design Principles

1. **Modularity**: Each phase is in a separate module for clarity
2. **Simplicity**: Uses straightforward algorithms suitable for learning
3. **Readability**: Code is well-commented and easy to understand
4. **Educational Focus**: Demonstrates compiler concepts without unnecessary complexity
5. **Standard C**: Uses only standard C libraries (stdio.h, stdlib.h, string.h, ctype.h)

## Learning Outcomes

Students will learn:
- How lexical analysis tokenizes source code
- How parsers validate syntax using grammars
- How symbol tables track variable information
- How intermediate code is generated
- How different compiler phases work together
- Error detection and reporting in compilers

## Limitations

As an educational project, this compiler has intentional limitations:
- Only supports integer data type
- No support for arrays, functions, or control structures
- No optimization phase
- No actual code generation (only intermediate code)
- Limited error recovery

## Future Enhancements (Optional)

Students can extend this project by adding:
- Support for float data type
- If-else statements
- While loops
- Functions
- Arrays
- Optimization phase
- Code generation (assembly or bytecode)

## Credits

This project is designed for undergraduate Compiler Design courses to help students understand fundamental compiler concepts through hands-on implementation.

## License

This is an educational project. Feel free to use and modify for learning purposes.

## Contact

For questions or issues, please open an issue on GitHub.