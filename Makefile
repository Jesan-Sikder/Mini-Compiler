# Mini Compiler Makefile
# Compiler Design Project

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = compiler
SRC_DIR = src
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/lexer.c $(SRC_DIR)/parser.c $(SRC_DIR)/symbol_table.c $(SRC_DIR)/intermediate_code.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = $(SRC_DIR)/lexer.h $(SRC_DIR)/parser.h $(SRC_DIR)/symbol_table.h $(SRC_DIR)/intermediate_code.h

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

# Run with test case 3 (extended features)
test3: $(TARGET)
	./$(TARGET) test3.txt

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
	@echo "  make test3   - Build and run with test3.txt"
	@echo "  make clean   - Remove build artifacts"
	@echo "  make rebuild - Clean and rebuild"
	@echo "  make help    - Show this help message"

.PHONY: all run test1 test2 test3 clean rebuild help
