# Mini Compiler - Compiler Design Project

## Project Title
**Design and Implementation of a Mini Compiler Using C**

## Overview
This is a mini compiler implementation in C for educational purposes. The project now follows a cleaner structure (`src/` and `docs/grammar/`) and demonstrates core compiler phases: lexical analysis, syntax analysis, symbol table management, intermediate code generation, a basic optimization pass, and simple bytecode emission.

## Features
- ✅ **Lexical Analysis**: Tokenizes source code into meaningful tokens
- ✅ **Syntax Analysis**: Validates grammar using recursive descent parsing
- ✅ **Symbol Table Management**: Tracks variable declarations and usage
- ✅ **Intermediate Code Generation**: Generates three-address code
- ✅ **Optimization Phase**: Performs basic constant folding on generated intermediate code
- ✅ **Code Generation Phase**: Emits stack-style bytecode-like output
- ✅ **Error Handling**: Detects and reports syntax and semantic errors

## Supported Language Features

### Data Types
- `int`
- `float`

### Statements
1. **Variable Declaration**
   ```c
   int a;
   int b = 10;
   float pi = 3.14;
   int nums[3];
   ```

2. **Assignment Statement**
   ```c
   a = b + 5;
   ```

3. **Arithmetic Expressions**
   - Operators: `+`, `-`, `*`, `/`
   - Example: `c = a + b * 2 - d / 3;`

4. **Control Flow**
   ```c
   if (a > 10) {
       print(a);
   } else {
       print(0);
   }

   while (a < 20) {
       a = a + 1;
   }
   ```

5. **Functions**
   ```c
   int add(int x, int y) {
       return x + y;
   }
   total = add(1, 2);
   ```

6. **Print Statement**
   ```c
   print(a);
   ```

### Language Rules
- Statements may span multiple lines (especially inside `{ ... }` blocks)
- Each statement must end with semicolon (`;`)
- Variables must be declared before use
- Follows standard operator precedence (`*`, `/` before `+`, `-`)

## Formal Grammar

```
program        → statement_list
statement_list → statement statement_list | statement

statement      → declaration | assignment_or_call | print_stmt
               | if_stmt | while_stmt | function_decl | return_stmt

declaration    → type identifier ;
               | type identifier = expression ;
               | type identifier [ number ] ;

type           → int | float

assignment_or_call → identifier = expression ;
                   | identifier [ expression ] = expression ;
                   | identifier ( arg_list ) ;

print_stmt     → print ( expression ) ;

if_stmt        → if ( condition ) block else_part
else_part      → else block | ε
while_stmt     → while ( condition ) block

function_decl  → type identifier ( param_list ) block
param_list     → type identifier (, type identifier)* | ε
arg_list       → expression (, expression)* | ε
return_stmt    → return expression ;
block          → { statement_list }

condition      → expression relop expression
relop          → == | != | < | > | <= | >=

expression     → term
               | expression + term
               | expression - term

term           → factor
               | term * factor
               | term / factor

factor         → identifier
               | identifier [ expression ]
               | identifier ( arg_list )
               | number
               | ( expression )
```

## Project Structure

```
Mini-compiler-/
├── src/
│   ├── main.c
│   ├── lexer.c
│   ├── lexer.h
│   ├── parser.c
│   ├── parser.h
│   ├── symbol_table.c
│   ├── symbol_table.h
│   ├── intermediate_code.c
│   └── intermediate_code.h
├── docs/
│   └── grammar/
│       └── GRAMMAR.md
├── Makefile
├── program.txt
├── test1.txt
├── test2.txt
├── test3.txt
└── README.md
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
gcc -Wall -Wextra -std=c99 -o compiler src/main.c src/lexer.c src/parser.c src/symbol_table.c src/intermediate_code.c
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

# Run with test3.txt (extended features)
make test3
```

#### Manual Execution
```bash
./compiler program.txt
./compiler test1.txt
./compiler test2.txt
./compiler test3.txt
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
- Function execution is represented at intermediate/bytecode level (no runtime VM yet)
- Array bounds are not dynamically checked
- Optimization is intentionally basic (constant folding only)
- Bytecode emission is educational and not tied to a real backend
- Error recovery is limited

## Future Enhancements (Optional)

Students can further extend this project by adding:
- Strong type checking (int/float coercion rules)
- Function scope stack frames and local scope isolation
- Better optimization passes (dead code elimination, CSE)
- Real backend code generation (assembly / VM executable format)
- More robust semantic analysis and error recovery

## Credits

This project is designed for undergraduate Compiler Design courses to help students understand fundamental compiler concepts through hands-on implementation.

## License

This is an educational project. Feel free to use and modify for learning purposes.

## Contact

For questions or issues, please open an issue on GitHub.
