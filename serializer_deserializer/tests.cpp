#include <sstream>
#include <cassert>
#include <iostream>
#include "serializer.hpp"
#include "deserializer.hpp"
#include "data.hpp"

// Тест успешной сериализации/десериализации
void test_basic_serialization() {
    Data x{1, true, 2};
    std::stringstream stream;
    
    // Сериализация
    Serializator serializer(stream);
    Error err = serializer.Save(x);
    assert(err == Error::NoError);
    
    // Десериализация
    Data y{0, false, 0};
    Deserializer deserializer(stream);
    err = deserializer.Load(y);
    
    assert(err == Error::NoError);
    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
    
    std::cout << "Basic serialization test passed\n";
}

// Тест поврежденного потока (пустой)
void test_corrupted_stream_empty() {
    Data y{0, false, 0};
    std::stringstream empty_stream;
    
    Deserializer deserializer(empty_stream);
    Error err = deserializer.Load(y);
    
    assert(err == Error::CorruptedArchive);
    std::cout << "Empty stream test passed\n";
}

// Тест неверного формата boolean
void test_invalid_bool_format() {
    Data y{0, false, 0};
    std::stringstream stream;
    stream << "1 unknown 2 ";  // Второе значение - неверный boolean
    
    Deserializer deserializer(stream);
    Error err = deserializer.Load(y);
    
    assert(err == Error::CorruptedArchive);
    std::cout << "Invalid boolean format test passed\n";
}

// Тест недостающих данных
void test_missing_data() {
    Data x{1, true, 2};
    std::stringstream stream;
    
    // Сериализация только первых двух полей
    Serializator serializer(stream);
    Error err = serializer.Save(x);
    assert(err == Error::NoError);
    
    // Урезаем поток
    std::string partial_data = stream.str().substr(0, stream.str().find_last_of(' '));
    std::stringstream modified_stream(partial_data);
    
    Data y{0, false, 0};
    Deserializer deserializer(modified_stream);
    err = deserializer.Load(y);
    
    assert(err == Error::CorruptedArchive);
    std::cout << "Missing data test passed\n";
}

// Тест лишних данных
void test_extra_data() {
    Data x{1, true, 2};
    std::stringstream stream;
    
    // Сериализация
    Serializator serializer(stream);
    Error err = serializer.Save(x);
    assert(err == Error::NoError);
    
    // Добавляем лишние данные
    stream << "extra_data";
    
    Data y{0, false, 0};
    Deserializer deserializer(stream);
    err = deserializer.Load(y);
    
    assert(err == Error::CorruptedArchive);
    std::cout << "Extra data test passed\n";
}

// Тест неверного формата числа
void test_invalid_number_format() {
    Data y{0, false, 0};
    std::stringstream stream;
    stream << "not_a_number true 2 ";  // Первое значение - не число
    
    Deserializer deserializer(stream);
    Error err = deserializer.Load(y);
    
    assert(err == Error::CorruptedArchive);
    std::cout << "Invalid number format test passed\n";
}

// Тест граничных значений
void test_edge_values() {
    Data x{0, false, 0};
    x.a = UINT64_MAX;
    x.c = UINT64_MIN;
    
    std::stringstream stream;
    Serializator serializer(stream);
    Error err = serializer.Save(x);
    assert(err == Error::NoError);
    
    Data y{0, true, 0};
    Deserializer deserializer(stream);
    err = deserializer.Load(y);
    
    assert(err == Error::NoError);
    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
    
    std::cout << "Edge values test passed\n";
}

int main() {
    test_basic_serialization();
    test_corrupted_stream_empty();
    test_invalid_bool_format();
    test_missing_data();
    test_extra_data();
    test_invalid_number_format();
    test_edge_values();
    
    std::cout << "All tests passed successfully!\n";
    return 0;
}