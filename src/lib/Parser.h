#include <string>
#include <variant>
#include <cstddef>

struct Data
{
    std::string name;
};

class Parser
{
public:
    // Returns std::variant<Data, std::string>:
    //   - Data on success
    //   - error message (std::string) on failure
    static std::variant<Data, std::string> parse(const char *buffer, std::size_t length);
};

struct Data2
{
    const char *name;
};

class Parser2
{
public:
    // Returns std::variant<Data, std::string>:
    //   - Data on success
    //   - error message (std::string) on failure
    static std::variant<Data2, std::string> parse(const char *buffer, std::size_t length);
};