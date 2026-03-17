# Mini Compiler Makefile
# Compiler Design Project

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = compiler
SOURCES = main.c lexer.c parser.c symbol_table.c intermediate_code.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = lexer.h parser.h symbol_table.h intermediate_code.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build successful! Executable: $(TARGET)"

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Run the compiler with default test program
run: $(TARGET)
	./$(TARGET) program.txt

# Run with test case 1
test1: $(TARGET)
	./$(TARGET) test1.txt

# Run with test case 2
test2: $(TARGET)
	./$(TARGET) test2.txt

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Rebuild from scratch
rebuild: clean all

# Help message
help:
	@echo "Mini Compiler - Available targets:"
	@echo "  make         - Build the compiler"
	@echo "  make run     - Build and run with program.txt"
	@echo "  make test1   - Build and run with test1.txt"
	@echo "  make test2   - Build and run with test2.txt"
	@echo "  make clean   - Remove build artifacts"
	@echo "  make rebuild - Clean and rebuild"
	@echo "  make help    - Show this help message"

.PHONY: all run test1 test2 clean rebuild help
