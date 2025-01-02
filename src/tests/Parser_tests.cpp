#include <gtest/gtest.h>
#include "Parser.h"

TEST(ParserTests, BasicAssertions)
{
    const char *buffer = "Hello, World!";
    auto parsed = Parser::parse_owned(buffer, strlen(buffer));

    ASSERT_TRUE(std::holds_alternative<DataOwned>(parsed));
    if (std::holds_alternative<DataOwned>(parsed))
    {
        const DataOwned &data = std::get<DataOwned>(parsed);
        ASSERT_EQ(data.name, "Hello, World!");
    }
    else
    {
        FAIL() << "Parsing failed with error: " << std::get<std::string>(parsed);
    }
}