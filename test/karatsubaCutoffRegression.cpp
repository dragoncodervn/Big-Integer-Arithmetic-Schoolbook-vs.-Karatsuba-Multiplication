#include "../karatsuba.hpp"
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

namespace
{
    std::string randomNumber(std::mt19937& generator, size_t length)
    {
        std::string number(length, '0');
        number[0] = static_cast<char>('1' + generator() % 9);
        for (size_t i = 1; i < length; ++i)
            number[i] = static_cast<char>('0' + generator() % 10);
        return number;
    }

    void requireEqual(const std::string& actual, const std::string& expected, const std::string& context)
    {
        if (actual != expected)
            throw std::runtime_error(context + "\nexpected: " + expected + "\nactual:   " + actual);
    }
}

int main()
{
    try
    {
        requireEqual(karatsuba("5017100403", "2792700000", 2),
                     "14011256295458100000", "Known cutoff=2 regression");
        requireEqual(subtract("0", "000"), "0", "Zero subtraction with leading zeros");
        requireEqual(add("000", "000"), "0", "Zero addition with leading zeros");

        std::mt19937 generator(20261027);
        for (size_t length = 3; length <= 12; ++length)
        {
            for (int trial = 0; trial < 1000; ++trial)
            {
                std::string lhs = randomNumber(generator, length);
                std::string rhs = randomNumber(generator, length);
                requireEqual(karatsuba(lhs, rhs, 2), multiply(lhs, rhs),
                             "Random cutoff=2 mismatch at " + std::to_string(length) + " digits");
            }
        }

        for (size_t length : {5000, 20000, 100000})
        {
            std::string lhs = randomNumber(generator, length);
            std::string rhs = randomNumber(generator, length);
            requireEqual(karatsuba(lhs, rhs, 2), karatsuba(lhs, rhs, 4),
                         "Large cutoff mismatch at " + std::to_string(length) + " digits");
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }

    std::cout << "Karatsuba cutoff regression checks passed." << std::endl;
    return 0;
}
