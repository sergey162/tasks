#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <optional>
#include <utility>



template <typename T>
concept OutputStreamable = requires(T&& value, std::ostream& os) {
    { os << value } -> std::same_as<std::ostream&>;
};

// Преобразование аргументов в строки с проверкой концепта
template <OutputStreamable T>
std::string ToString(T&& value) {
    std::ostringstream oss;
    oss << std::forward<T>(value);
    return oss.str();
}

std::optional<size_t> ParseIndex(const std::string& input, size_t start_pos, size_t* end_pos) {
    size_t index = 0;
    bool digit_found = false;
    size_t pos = start_pos;

    while (pos < input.size() && std::isdigit(static_cast<unsigned char>(input[pos]))) {
        digit_found = true;
        index = index * 10 + (input[pos] - '0');
        pos++;
    }

    if (!digit_found || pos >= input.size() || input[pos] != '}') {
        return std::nullopt;
    }

    *end_pos = pos;
    return index;
}

template <typename... Args>
std::string format(const std::string& input, Args&&... args) {
    std::vector<std::string> words;
    (words.push_back(ToString(std::forward<Args>(args))), ...);
    std::string result;
    size_t i = 0;
    const size_t n = input.size();
    while (i < n) {
        if (input[i] == '{') {
            size_t end_pos;
            if (auto index = ParseIndex(input, i + 1, &end_pos)) {
                if (*index >= words.size()) {
                    throw std::out_of_range("Argument index " + std::to_string(*index) + 
                                          " out of range [0, " + std::to_string(words.size() - 1) + "]");
                }
                result += words[*index];
                i = end_pos + 1;
            } else {
                throw std::runtime_error("Invalid format at position " + std::to_string(i));
            }
        } else if (input[i] == '}') {
            throw std::runtime_error("Unmatched at position " + std::to_string(i));
        } else {
            result.push_back(input[i++]);
        }
    }
    return result;
}
