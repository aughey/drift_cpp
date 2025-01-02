#include <string>
#include <variant>
#include <cstddef>

struct DataOwned
{
    std::string name;
};

struct DataShared
{
    std::string_view name;
};

class Parser
{
public:
    // Returns std::variant<Data, std::string>:
    //   - Data on success
    //   - error message (std::string) on failure
    static std::variant<DataOwned, std::string> parse_owned(const char *buffer, std::size_t length);
    // Returns std::variant<Data, std::string>:
    //   - Data on success
    //   - error message (std::string) on failure
    static std::variant<DataShared, std::string> parse_shared(const char *buffer, std::size_t length);
};