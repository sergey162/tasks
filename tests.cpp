#include "format.h"
#include <cassert>
#include <stdexcept>

void EmptyString() {
    std::string input;
    assert(format(input).empty());
}

void StringWithOnlyLeftParentnes() {
    std::string input = "aba{0aba{";
    try {
        format(input);
        assert(false);
    } catch(const std::runtime_error& e) {
        // Ожидаемое исключение
    }
}

void StringWithOnlyRightParentnes() {
    std::string input = "aba}caba}";
    try {
        format(input);
        assert(false);
    } catch(const std::runtime_error& e) {
        // Ожидаемое исключение
    }
}

void IncorrectIndexInArgsPack() {
    try {
        // Недостаточно аргументов для подстановки
        format("first number is: {0}", 0, 1, 2); // Корректный вызов
        format("index out of range: {5}", 0, 1, 2); // Индекс 5 при 3 аргументах
        assert(false);
    } catch (const std::out_of_range& e) {
        // Ожидаемое исключение
    }
}

void TestCorrectFormatting() {
    // Базовый тест
    auto text1 = format("{1}+{1} = {0}", 2, "one");
    assert(text1 == "one+one = 2");
    
    // Разные типы аргументов
    auto text2 = format("int: {0}, double: {1}, char: {2}", 42, 3.14, 'A');
    assert(text2 == "int: 42, double: 3.14, char: A");
    
    // Повторное использование аргументов
    auto text3 = format("{0}{1}{0}", "a", "b");
    assert(text3 == "aba");
    
    // Строка без подстановок
    auto text4 = format("No substitutions", 10, 20);
    assert(text4 == "No substitutions");
}

void TestEdgeCases() {
    // Пустые аргументы
    auto text1 = format("{0}{1}", "", "");
    assert(text1 == "");
    
    // Ведущие нули в индексе
    auto text2 = format("{01}{00}", "a", "b");
    assert(text2 == "ba");
    
    // Большой индекс
    auto text3 = format("{2}", 0, 1, 2, 3, 4, 5);
    assert(text3 == "2");
}

void TestErrorHandling() {
    // Пустые скобки
    try {
        format("empty {} braces", 42);
        assert(false);
    } catch (const std::runtime_error& e) {}
    
    // Нечисловой индекс
    try {
        format("{a}", 42);
        assert(false);
    } catch (const std::runtime_error& e) {}
    
    // Незакрытая скобка
    try {
        format("{0", 42);
        assert(false);
    } catch (const std::runtime_error& e) {}
    
    // Вложенные скобки
    try {
        format("{0{1}}", 1, 2);
        assert(false);
    } catch (const std::runtime_error& e) {}
    
    // Отрицательный индекс
    try {
        format("{-1}", 42);
        assert(false);
    } catch (const std::runtime_error& e) {}
    
    // Пропущенные аргументы
    try {
        format("{0}");
        assert(false);
    } catch (const std::out_of_range& e) {}
}

void TestNoArguments() {
    // Без аргументов и без подстановок
    assert(format("Simple text") == "Simple text");
    
    // Без аргументов, но с подстановками
    try {
        format("{0}");
        assert(false);
    } catch (const std::out_of_range& e) {}
}


void TestMixedBraces() {
    // Корректное использование с обычными скобками
    auto text1 = format("Valid {0} and character", "test");
    assert(text1 == "Valid test and character");
    
    // Ошибочное сочетание
    try {
        format("Invalid {0} and } without corresponding {", "test");
        assert(false);
    } catch (const std::runtime_error& e) {}
}

void TestFromGithub() {
    auto text = format("{1}+{1} = {0}", 2, "one");
    assert(text == "one+one = 2");
}

int main() {
    EmptyString();
    StringWithOnlyLeftParentnes();
    StringWithOnlyRightParentnes();
    IncorrectIndexInArgsPack();
    TestCorrectFormatting();
    TestEdgeCases();
    TestErrorHandling();
    TestNoArguments();
    TestMixedBraces();
    TestFromGithub();
    return 0;
}