#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "serializer.hpp"
#include "deserializer.hpp"
#include "data.hpp"

TEST(SerializationTest, Basic) {
    Data x{1, true, 2};
    std::stringstream stream;
    
    Serializer serializer(stream);
    Error err = serializer.Save(x);
    ASSERT_EQ(err, Error::NoError);
    
    Data y{0, false, 0};
    Deserializer deserializer(stream);
    err = deserializer.Load(y);
    
    ASSERT_EQ(err, Error::NoError);
    EXPECT_EQ(x.a, y.a);
    EXPECT_EQ(x.b, y.b);
    EXPECT_EQ(x.c, y.c);
}

TEST(SerializationTest, CorruptedStreamEmpty) {
    Data y{0, false, 0};
    std::stringstream empty_stream;
    
    Deserializer deserializer(empty_stream);
    Error err = deserializer.Load(y);
    
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST(SerializationTest, InvalidBoolFormat) {
    Data y{0, false, 0};
    std::stringstream stream;
    stream << "1 unknown 2 ";
    
    Deserializer deserializer(stream);
    Error err = deserializer.Load(y);
    
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST(SerializationTest, MissingData) {
    Data x{1, true, 2};
    std::stringstream stream;
    
    Serializer serializer(stream);
    Error err = serializer.Save(x);
    ASSERT_EQ(err, Error::NoError);
    
    std::string partial_data = stream.str();
    size_t last_space = partial_data.find_last_of(' ');
    partial_data = partial_data.substr(0, last_space);
    
    std::stringstream modified_stream(partial_data);
    Data y{0, false, 0};
    Deserializer deserializer(modified_stream);
    err = deserializer.Load(y);
    
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST(SerializationTest, ExtraData) {
    Data x{1, true, 2};
    std::stringstream stream;
    
    Serializer serializer(stream);
    Error err = serializer.Save(x);
    ASSERT_EQ(err, Error::NoError);
    
    stream << " extra_data";
    
    Data y{0, false, 0};
    Deserializer deserializer(stream);
    err = deserializer.Load(y);
    
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST(SerializationTest, InvalidNumberFormat) {
    Data y{0, false, 0};
    std::stringstream stream;
    stream << "not_a_number true 2";  
    
    Deserializer deserializer(stream);
    Error err = deserializer.Load(y);
    
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST(SerializationTest, EdgeValues) {
    Data x{0, false, 0};
    x.a = std::numeric_limits<uint64_t>::max();
    x.c = 0; 
    
    std::stringstream stream;
    Serializer serializer(stream);
    Error err = serializer.Save(x);
    ASSERT_EQ(err, Error::NoError);
    
    Data y{0, true, 1};
    Deserializer deserializer(stream);
    err = deserializer.Load(y);
    
    ASSERT_EQ(err, Error::NoError);
    EXPECT_EQ(x.a, y.a);
    EXPECT_EQ(x.b, y.b);
    EXPECT_EQ(x.c, y.c);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
