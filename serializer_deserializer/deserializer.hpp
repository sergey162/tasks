#pragma once


#include <istream>
#include <cstdint>
#include <string>
#include "error.hpp"

class Deserializer {
public:
    explicit Deserializer(std::istream& in) : in_(in) {}

    template <class T>
    Error Load(T& object) {
        return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args&&... args) {
        return Process(std::forward<Args>(args)...);
    }

private:
    std::istream& in_;

    template <class T>
    Error Process(T&& arg) {
        return ProcessValue(std::forward<T>(arg));
    }

    template <class T, class... Args>
    Error Process(T&& arg, Args&&... args) {
        if (auto err = ProcessValue(std::forward<T>(arg)); err != Error::NoError) {
            return err;
        }
        return Process(std::forward<Args>(args)...);
    }

    Error ProcessValue(uint64_t& value) {
        std::string text;
        in_ >> text;
        try {
            value = std::stoull(text);
        } catch (...) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error ProcessValue(bool& value) {
        std::string text;
        in_ >> text;
        if (text == "true") {
            value = true;
        } else if (text == "false") {
            value = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
};