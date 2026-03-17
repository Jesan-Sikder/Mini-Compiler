# QUICK START GUIDE

## Installation and Setup

### Prerequisites
- GCC compiler (or any C compiler)
- Make utility (optional but recommended)
- Text editor

### Quick Setup
```bash
# Clone the repository (or download)
cd raha-compiler-

# Build the compiler
make

# Run with sample program
make run
```

---

## Usage Examples

### Example 1: Basic Declaration and Assignment
**Input file (test1.txt)**:
```c
int x = 5;
int y = 10;
int sum;
sum = x + y;
print(sum);
```

**Run**:
```bash
./compiler test1.txt
```

**Expected Output**: Compilation successful with intermediate code showing `sum = 15`

---

### Example 2: Complex Expression
**Input file (test2.txt)**:
```c
int a = 10;
int b = 20;
int c = 5;
int result;
result = a + b * c - 15;
print(result);
```

**Run**:
```bash
./compiler test2.txt
```

**Note**: Operator precedence is respected: `b * c` is computed before addition/subtraction

---

### Example 3: Multiple Operations
**Input file (test3.txt)**:
```c
int x = 100;
int y = 50;
int z;
z = x / y + 10 * 2;
print(z);
```

**Run**:
```bash
./compiler test3.txt
```

---

## Common Errors and Solutions

### Error 1: Undeclared Variable
**Code**:
```c
a = 10;  // Error: 'a' not declared
```

**Solution**: Declare variable first
```c
int a;
a = 10;  // Correct
```

---

### Error 2: Missing Semicolon
**Code**:
```c
int a = 10  // Error: Missing semicolon
```

**Solution**: Add semicolon
```c
int a = 10;  // Correct
```

---

### Error 3: Duplicate Declaration
**Code**:
```c
int a;
int a;  // Error: 'a' already declared
```

**Solution**: Remove duplicate
```c
int a;  // Correct
```

---

### Error 4: Wrong Syntax for Print
**Code**:
```c
print a;  // Error: Missing parentheses
```

**Solution**: Use parentheses
```c
print(a);  // Correct
```

---

## Language Syntax Reference

### Declaration
```c
int variableName;              // Declaration without initialization
int variableName = value;      // Declaration with initialization
```

### Assignment
```c
variableName = expression;
```

### Expression
```c
// Arithmetic operators (in order of precedence)
a * b    // Multiplication (highest)
a / b    // Division (highest)
a + b    // Addition (lower)
a - b    // Subtraction (lower)

// Complex expressions
result = a + b * c;           // b*c computed first
result = (a + b) / c;         // NOT SUPPORTED - no parentheses in expressions
```

### Print Statement
```c
print(variableName);
```

---

## Tips for Students

1. **Start Simple**: Begin with simple programs and gradually add complexity

2. **One Statement Per Line**: Keep your code readable
   ```c
   // Good
   int a;
   int b = 10;
   
   // Avoid
   int a; int b = 10;
   ```

3. **Always Declare Before Use**:
   ```c
   // Correct order
   int x;
   x = 5;
   
   // Wrong order
   x = 5;  // Error!
   int x;
   ```

4. **Check for Semicolons**: Every statement must end with `;`

5. **Use Meaningful Names**:
   ```c
   // Good
   int total;
   int count;
   
   // Less clear
   int t;
   int c;
   ```

6. **Test Your Code**: Run your program through the compiler to check for errors

---

## Debugging Tips

### Problem: Compiler doesn't compile
**Solution**:
```bash
# Check GCC is installed
gcc --version

# Rebuild from scratch
make clean
make
```

### Problem: Cannot open input file
**Solution**:
```bash
# Make sure file exists
ls -l program.txt

# Check file path is correct
./compiler program.txt  # not ./compiler program
```

### Problem: Many syntax errors
**Solution**:
- Check each statement ends with semicolon
- Verify all variables are declared
- Check print statement has parentheses
- Review the sample programs

---

## Make Commands Reference

```bash
make           # Build the compiler
make run       # Build and run with program.txt
make test1     # Build and run with test1.txt
make test2     # Build and run with test2.txt
make clean     # Remove compiled files
make rebuild   # Clean and rebuild
make help      # Show help message
```

---

## Creating Your Own Test Files

1. Create a new text file:
   ```bash
   nano myprogram.txt
   # or use any text editor
   ```

2. Write your program following the language syntax

3. Save the file

4. Run the compiler:
   ```bash
   ./compiler myprogram.txt
   ```

---

## Understanding Compiler Output

The compiler shows 4 phases of output:

### 1. Token List
Shows how your code is broken into tokens
```
INT IDENTIFIER(a) ASSIGN NUMBER(10) SEMICOLON
```

### 2. Syntax Validation
Indicates if syntax is correct or shows errors

### 3. Symbol Table
Shows all variables with their properties

### 4. Intermediate Code
Three-address code representation of your program

---

## Sample Programs

### Program 1: Simple Calculator
```c
int num1 = 25;
int num2 = 5;
int sum;
int diff;
int prod;
int quot;
sum = num1 + num2;
diff = num1 - num2;
prod = num1 * num2;
quot = num1 / num2;
print(sum);
print(diff);
print(prod);
print(quot);
```

### Program 2: Expression Evaluation
```c
int a = 10;
int b = 5;
int c = 2;
int result;
result = a + b * c;
print(result);
```

### Program 3: Sequential Operations
```c
int base = 100;
int increment = 10;
int temp;
temp = base + increment;
base = temp;
print(base);
```

---

## FAQs

**Q: Can I use float or double?**
A: No, only `int` is supported in this version.

**Q: Can I use if-else or loops?**
A: No, this mini compiler doesn't support control structures.

**Q: Can I use parentheses in expressions?**
A: No, but operator precedence is handled automatically.

**Q: Can I declare multiple variables in one line?**
A: No, use separate lines:
```c
int a;
int b;
int c;
```

**Q: Do I need to initialize variables?**
A: No, but it's good practice:
```c
int x;      // Valid but uninitialized
int y = 0;  // Better
```

---

## Need Help?

1. Check the README.md for detailed documentation
2. Review COMPILER_PHASES.md for understanding how it works
3. Look at the sample programs (program.txt, test1.txt, test2.txt)
4. Check error messages carefully - they tell you what's wrong and where

---

**Happy Coding!** 💻
