#include <iostream>
#include "Parser.h"
#include <string.h>
#include <memory>
#include <assert.h>

// Given cognitive drift, the interaction between the buffer and parser is not clear.
// One of the following must be true for correct and safe operation:
// 1. The developer get's lucky and doesn't break anything.
// 2. The developer has a deep understanding of the parser and buffer interaction (already said isn't true)
// 3. The developer has:
//    a. Read and understands the documentation on the buffer
//    b. Read and understands the documentation on the parser
//    c. Read the documentation on the interaction between the buffer and parser
//    d. The documentation is current, complete, and correct.

int main(int, char *[])
{
    const char *hello_world = "Hello, World!";

    {
        auto parsed = Parser::parse_owned(hello_world, strlen(hello_world));
        if (std::holds_alternative<DataOwned>(parsed))
        {
            const DataOwned &data = std::get<DataOwned>(parsed);
            std::cout << "Parser data: " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::string(hello_world);
        assert(buffer.c_str() != hello_world);
        auto parsed = Parser::parse_owned(buffer.c_str(), buffer.length());

        if (std::holds_alternative<DataOwned>(parsed))
        {
            const DataOwned &data = std::get<DataOwned>(parsed);
            std::cout << "Parser data: " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::string(hello_world);
        assert(buffer.c_str() != hello_world);
        auto parsed = Parser::parse_shared(buffer.c_str(), buffer.length());

        if (std::holds_alternative<DataShared>(parsed))
        {
            const DataShared &data = std::get<DataShared>(parsed);
            std::cout << "Parser data: " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::string(hello_world);
        assert(buffer.c_str() != hello_world);
        auto parsed = Parser::parse_owned(buffer.c_str(), buffer.length());

        // As a pipeline, we reuse the buffer to get the next data from the network.
        buffer[0] = 'J';

        if (std::holds_alternative<DataOwned>(parsed))
        {
            const auto &data = std::get<DataOwned>(parsed);
            std::cout << "Parser data (should be \"Hello, World!\"): " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::string(hello_world);
        assert(buffer.c_str() != hello_world);
        auto parsed = Parser::parse_shared(buffer.c_str(), buffer.length());

        // As a pipeline, we reuse the buffer to get the next data from the network.
        buffer[0] = 'J';

        if (std::holds_alternative<DataShared>(parsed))
        {
            const auto &data = std::get<DataShared>(parsed);
            // This is bad, because the internals of buffer might have allocated a new string or reused the old one.
            // No matter what, we're on thin ice here and not guaranteed to get the data we expect or even worse
            // we might get a crash or other undefined behavior.
            //
            // Valgrind doesn't catch this.
            std::cout << "Parser data (should be \"Hello World!\"): " << data.name << std::endl;
        }
    }
    {
        auto buffer = std::string(hello_world);
        assert(buffer.c_str() != hello_world);
        auto parsed = Parser::parse_shared(buffer.c_str(), buffer.length());

        // As a pipeline, we reuse the buffer to get the next data from the network.
        // This should cause a reallocation of buffer because the new string is notably longer.
        buffer.assign("The quick brown fox jumps over the lazy dog");

        if (std::holds_alternative<DataShared>(parsed))
        {
            const auto &data = std::get<DataShared>(parsed);
            // This is bad, because the internals of buffer might have allocated a new string or reused the old one.
            // No matter what, we're on thin ice here and not guaranteed to get the data we expect or even worse
            // we might get a crash or other undefined behavior.
            //
            // Valgrind doesn't catch this if the buffer isn't reallocated.
            std::cout << "Parser data (should be \"Hello World!\"): " << data.name << std::endl;
        }
    }

    {
        auto buffer = std::make_unique<std::string>(hello_world);
        assert(buffer->c_str() != hello_world);
        auto parsed = Parser::parse_owned(buffer->c_str(), buffer->length());
        // Completely deallocate the orignal buffer.
        buffer.reset();

        if (std::holds_alternative<DataOwned>(parsed))
        {
            const auto &data = std::get<DataOwned>(parsed);
            std::cout << "Since Parser makes a copy of the data, this is fine: " << data.name << std::endl;
        }
    }

    if (true)
    {
        auto buffer = std::make_unique<std::string>(hello_world);
        assert(buffer->c_str() != hello_world);
        auto parsed = Parser::parse_shared(buffer->c_str(), buffer->length());
        // Completely deallocate the orignal buffer.
        buffer.reset();

        if (std::holds_alternative<DataShared>(parsed))
        {
            const DataShared &data = std::get<DataShared>(parsed);
            std::cout << "This will probably crash: " << data.name << std::endl;
        }
    }
    std::cout << "Done!" << std::endl;

    return 0;
}