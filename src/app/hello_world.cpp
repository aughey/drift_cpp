#include <iostream>
#include "Parser.h"
#include <string.h>
#include <memory>
#include <assert.h>

int main(int, char *[])
{
    const char *hello_world = "Hello, World!";

    {
        auto parsed = Parser::parse(hello_world, strlen(hello_world));
        if (std::holds_alternative<Data>(parsed))
        {
            const Data &data = std::get<Data>(parsed);
            std::cout << "Parser data: " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::string(hello_world);
        assert(buffer.c_str() != hello_world);
        auto parsed = Parser::parse(buffer.c_str(), buffer.length());

        if (std::holds_alternative<Data>(parsed))
        {
            const Data &data = std::get<Data>(parsed);
            std::cout << "Parser data: " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::string(hello_world);
        assert(buffer.c_str() != hello_world);
        auto parsed = Parser2::parse(buffer.c_str(), buffer.length());

        if (std::holds_alternative<Data2>(parsed))
        {
            const Data2 &data = std::get<Data2>(parsed);
            std::cout << "Parser2 data 2: " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::string(hello_world);
        assert(buffer.c_str() != hello_world);
        auto parsed = Parser::parse(buffer.c_str(), buffer.length());

        // As a pipeline, we reuse the buffer to get the next data from the network.
        buffer.assign("Hello, World 2!");

        if (std::holds_alternative<Data>(parsed))
        {
            const auto &data = std::get<Data>(parsed);
            std::cout << "Parser data (should be \"Hello, World!\"): " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::string(hello_world);
        assert(buffer.c_str() != hello_world);
        auto parsed = Parser2::parse(buffer.c_str(), buffer.length());

        std::cout << "Internal buffer len of string is: " << buffer.capacity() << " and len is " << buffer.length() << std::endl;

        // As a pipeline, we reuse the buffer to get the next data from the network.
        buffer.assign("Hello, World 2!");

        if (std::holds_alternative<Data2>(parsed))
        {
            const auto &data = std::get<Data2>(parsed);
            // This is bad, because the internals of buffer might have allocated a new string or reused the old one.
            // No matter what, we're on thin ice here and not guaranteed to get the data we expect or even worse
            // we might get a crash or other undefined behavior.
            //
            // Valgrind doesn't catch this.
            std::cout << "Parser2 data (should be \"Hello World!\"): " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::make_unique<std::string>(hello_world);
        assert(buffer->c_str() != hello_world);
        auto parsed = Parser::parse(buffer->c_str(), buffer->length());
        buffer.reset();

        if (std::holds_alternative<Data>(parsed))
        {
            const auto &data = std::get<Data>(parsed);
            std::cout << "Since Parser makes a copy of the data, this is fine: " << data.name << std::endl;
        }
    }

    if (false)
    {
        auto buffer = std::make_unique<std::string>(hello_world);
        assert(buffer->c_str() != hello_world);
        auto parsed = Parser2::parse(buffer->c_str(), buffer->length());
        buffer.reset();

        if (std::holds_alternative<Data2>(parsed))
        {
            const Data2 &data = std::get<Data2>(parsed);
            std::cout << "This will probably crash: " << data.name << std::endl;
        }
    }
    std::cout << "Done!" << std::endl;

    return 0;
}