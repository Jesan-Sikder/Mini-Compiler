# Mini Compiler Grammar (Current)

```bnf
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
return_stmt    → return expression ;

param_list     → type identifier (, type identifier)* | ε
arg_list       → expression (, expression)* | ε
block          → { statement_list }
condition      → expression relop expression
relop          → == | != | < | > | <= | >=

expression     → term | expression + term | expression - term
term           → factor | term * factor | term / factor
factor         → identifier
               | identifier [ expression ]
               | identifier ( arg_list )
               | number
               | ( expression )
```
