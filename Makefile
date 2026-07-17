
ifeq ($(OS),Windows_NT)

    CXX = g++
    TARGET = build/library_app.exe
    OBJ_DIR = build
    SRC_DIR = src
    INC_DIR = include
else

    CXX = g++
    TARGET = build/library_app
    OBJ_DIR = build
    SRC_DIR = src
    INC_DIR = include
endif


CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2
INCLUDES = -Iinclude

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)
	@echo "Build successful: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	@echo "Cleaning build files..."
	@rm -f $(OBJ_DIR)/*.o $(TARGET)
	@echo "Clean complete."

rebuild: clean all

help:
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run the project"
	@echo "  make clean    - Clean build files"
	@echo "  make rebuild  - Clean and rebuild"

.PHONY: all clean run rebuild help
