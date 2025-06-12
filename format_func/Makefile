CXX := g++
CXXFLAGS := -std=c++20 -Wall -Werror

TEST_SRC := tests.cpp
TEST_EXE := test_program

GTEST_DIR := gtest
GTEST_REPO := https://github.com/google/googletest.git 
GTEST_BUILD_DIR := $(GTEST_DIR)/build
GTEST_LIB := $(GTEST_BUILD_DIR)/lib/libgtest.a
GTEST_INCLUDE := $(GTEST_DIR)/googletest/include

all: $(TEST_EXE)

$(TEST_EXE): $(TEST_SRC) format.h $(GTEST_LIB)
    $(CXX) $(CXXFLAGS) -isystem $(GTEST_INCLUDE) -o $@ $< $(GTEST_LIB) -lpthread


$(GTEST_LIB):
    @echo "Building Google Test..."
    mkdir -p $(GTEST_BUILD_DIR)
    cd $(GTEST_BUILD_DIR) && cmake ../googletest && cmake --build .


download_gtest:
    @echo "Downloading Google Test..."
    git clone $(GTEST_REPO) $(GTEST_DIR)


test: $(TEST_EXE)
    @echo "Running tests..."
    @./$(TEST_EXE) && echo "All tests passed!" || (echo "Tests failed!"; exit 1)

clean:
    rm -f $(TEST_EXE)

clean_all: clean
    rm -rf $(GTEST_DIR)

.PHONY: all test clean clean_all download_gtest