#include <gtest/gtest.h>
#include "Parser.h"

TEST(ParserTests, BasicAssertions)
{
    const char *buffer = "Hello, World!";
    auto parsed = Parser::parse(buffer, strlen(buffer));

    ASSERT_TRUE(std::holds_alternative<Data>(parsed));
    if (std::holds_alternative<Data>(parsed))
    {
        const Data &data = std::get<Data>(parsed);
        ASSERT_EQ(data.name, "Hello, World!");
    }
    else
    {
        FAIL() << "Parsing failed with error: " << std::get<std::string>(parsed);
    }
}