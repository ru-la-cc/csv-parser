CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
SRC = test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = build/test

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "build complete"
	@rm -f $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
	@echo "clean complete"
