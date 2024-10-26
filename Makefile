# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/Cellar/fltk/1.3.9/include
LDFLAGS = -L/opt/homebrew/Cellar/fltk/1.3.9/lib -lfltk

# Directories
BUILD_DIR = ./build/
OBJS_DIR = $(BUILD_DIR)locals/
OUTPUT_DIR = $(BUILD_DIR)

# Target executable
TARGET = game

# Source files
SRCS = $(wildcard *.cpp)

# Object files (place in build directory)
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

all: $(BUILD_DIR) $(TARGET)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJS_DIR)

# Rule to build executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(OUTPUT_DIR)$(TARGET) $(LDFLAGS)

# Rule to compile .cpp files into .o files in the build directory
$(OBJS_DIR)%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	open $(OUTPUT_DIR)$(TARGET)
clean:
	rm -rf $(BUILD_DIR)