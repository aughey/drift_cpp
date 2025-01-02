#include "Parser.h"
#include <iostream>

// Returns std::variant<Data, std::string>:
//   - Data on success
//   - error message (std::string) on failure
std::variant<Data, std::string> Parser::parse(const char *buffer, std::size_t length)
{
    // First, check if all characters are in the ASCII range [0..127].
    for (std::size_t i = 0; i < length; ++i)
    {
        unsigned char c = static_cast<unsigned char>(buffer[i]);
        if (c > 127)
        {
            return std::string("Error: Non-ASCII character found at index ") + std::to_string(i);
        }
    }

    // If it's all ASCII, construct the Data object
    Data result;
    // Here we treat the entire buffer (length bytes) as the name.
    result.name.assign(buffer, length);

    // Return the successfully parsed Data
    return result;
}

// Returns std::variant<Data, std::string>:
//   - Data on success
//   - error message (std::string) on failure
std::variant<Data2, std::string> Parser2::parse(const char *buffer, std::size_t length)
{
    // First, check if all characters are in the ASCII range [0..127].
    for (std::size_t i = 0; i < length; ++i)
    {
        unsigned char c = static_cast<unsigned char>(buffer[i]);
        if (c > 127)
        {
            return std::string("Error: Non-ASCII character found at index ") + std::to_string(i);
        }
    }

    // If it's all ASCII, construct the Data object
    Data2 result;
    result.name = buffer;

    // Return the successfully parsed Data
    return result;
}