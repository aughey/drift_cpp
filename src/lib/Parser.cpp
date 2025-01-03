#include "Parser.h"
#include <iostream>

// Parses the incoming data buffer and returns std::variant<Data, std::string>:
//   - Data on success
//   - error message (std::string) on failure
std::variant<DataOwned, std::string> Parser::parse_owned(const char *buffer, std::size_t length)
{
    // First, check if all characters are in the ASCII range [0..127].
    for (std::size_t i = 0; i < length; ++i)
    {
        if (!isascii(buffer[i]))
        {
            return std::string("Error: Non-ASCII character found at index ") + std::to_string(i);
        }
    }

    DataOwned result;
    // Here we treat the entire buffer (length bytes) as the name.
    result.name.assign(buffer, length);

    // Return the successfully parsed Data
    return result;
}

// Returns std::variant<Data, std::string>:
//   - Data on success
//   - error message (std::string) on failure
std::variant<DataShared, std::string> Parser::parse_shared(const char *buffer, std::size_t length)
{
    // First, check if all characters are in the ASCII range [0..127].
    for (std::size_t i = 0; i < length; ++i)
    {
        if (!isascii(buffer[i]))
        {
            return std::string("Error: Non-ASCII character found at index ") + std::to_string(i);
        }
    }

    // Since we're pointed to shared data, the length index must be null
    // terminated, so we can't use it directly.
    if (buffer[length] != '\0')
    {
        return std::string("Error: buffer passed to parse_shared not null terminated");
    }

    DataShared result;
    // result.name = std::string_view(buffer, length);
    result.name = buffer;

    // Return the successfully parsed Data
    return result;
}