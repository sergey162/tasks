CXX := g++
CXXFLAGS := -std=c++20 -Wall -Werror

TEST_SRC := tests.cpp
TEST_EXE := test_program

all: $(TEST_EXE)

$(TEST_EXE): $(TEST_SRC) format.h
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SRC)


test: $(TEST_EXE)
	@echo "Running tests..."
	@./$(TEST_EXE) && echo "All tests passed!" || (echo "Tests failed!"; exit 1)

clean:
	rm -f $(TEST_EXE)

.PHONY: all test clean