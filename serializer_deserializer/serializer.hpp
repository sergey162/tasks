#pragma once


#include <ostream>
#include <cstdint>
#include <string>

#include "error.hpp"

class Serializator {
    static constexpr char Separator = ' ';
public:
    Serializator(std::ostream& os) : os_(os) {}

    template <class T>
    Error Save(T& object) {
        return object.serialize(*this);
    }

    template <typename Args>
    Error operator()(Args&&... args) {
        return Process(std::forward<Args>(args)...);
    }

private:

    Error ProcessValue(uint64_t&& value) {
        os_ << value << Separator;
        return Error::NoError;
    }

    Error ProcessValue(bool&& value) {
        os_ << (value) ? ("true") : ("false") << Separator;
        return Error::NoError;
    }

    template <typename T, typename... Args>
    Error Process(T&& val, Args&&... args) {
        if (auto err = ProcessValue(std::forward<T>(arg)); err != Error::NoError) {
            return err;
        }
        return Process(std::forward<Args>(args)...);
    }
private:
    std::ostream& os_;
};