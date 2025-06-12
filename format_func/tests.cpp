#include "format.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

// Тест: пустая строка
TEST(FormatTest, EmptyString) {
    std::string input;
    EXPECT_TRUE(format(input).empty());
}

// Тест: только левые скобки
TEST(FormatTest, LeftBracesOnly) {
    std::string input = "aba{0aba{";
    EXPECT_THROW(format(input), std::runtime_error);
}

// Тест: только правые скобки
TEST(FormatTest, RightBracesOnly) {
    std::string input = "aba}caba}";
    EXPECT_THROW(format(input), std::runtime_error);
}

// Тест: индекс аргумента выходит за границы
TEST(FormatTest, IndexOutOfRange) {
    EXPECT_THROW({
        format("index out of range: {5}", 0, 1, 2);
    }, std::out_of_range);
}

// Тест: корректное форматирование
TEST(FormatTest, CorrectFormatting) {
    EXPECT_EQ(format("{1}+{1} = {0}", 2, "one"), "one+one = 2");
    EXPECT_EQ(format("int: {0}, double: {1}, char: {2}", 42, 3.14, 'A'),
              "int: 42, double: 3.14, char: A");
    EXPECT_EQ(format("{0}{1}{0}", "a", "b"), "aba");
    EXPECT_EQ(format("No substitutions", 10, 20), "No substitutions");
}

// Тест: граничные случаи
TEST(FormatTest, EdgeCases) {
    EXPECT_EQ(format("{0}{1}", "", ""), "");
    EXPECT_EQ(format("{01}{00}", "a", "b"), "ba");
    EXPECT_EQ(format("{2}", 0, 1, 2, 3, 4, 5), "2");
}

// Тест: обработка ошибок
TEST(FormatTest, ErrorHandling) {
    EXPECT_THROW(format("empty {} braces", 42), std::runtime_error);  // Пустые {}
    EXPECT_THROW(format("{a}", 42), std::runtime_error);              // Не число
    EXPECT_THROW(format("{0", 42), std::runtime_error);               // Незакрытая {
    EXPECT_THROW(format("{0{1}}", 1, 2), std::runtime_error);         // Вложенные {}
    EXPECT_THROW(format("{-1}", 42), std::runtime_error);             // Отрицательный индекс
    EXPECT_THROW(format("{0}"), std::out_of_range);                   // Нет аргумента
}

// Тест: отсутствие аргументов
TEST(FormatTest, NoArguments) {
    EXPECT_EQ(format("Simple text"), "Simple text");
    EXPECT_THROW(format("{0}"), std::out_of_range);
}

// Тест: смешанные скобки
TEST(FormatTest, MixedBraces) {
    EXPECT_EQ(format("Valid {0} and character", "test"), "Valid test and character");
    EXPECT_THROW(format("Invalid {0} and } without corresponding {", "test"),
                 std::runtime_error);
}

// Тест из GitHub
TEST(FormatTest, GitHubExample) {
    EXPECT_EQ(format("{1}+{1} = {0}", 2, "one"), "one+one = 2");
}

// Точка входа для запуска тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}