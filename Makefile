CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
SRC = test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = build/test.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@if not exist build mkdir build
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "build complete"
	@del /Q $(OBJ) 2>nul || echo "Object files cleaned"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@del /Q $(OBJ) 2>nul || echo "No object files to clean"
	@del /Q $(TARGET) 2>nul || echo "No target to clean"
	@echo "clean complete"
