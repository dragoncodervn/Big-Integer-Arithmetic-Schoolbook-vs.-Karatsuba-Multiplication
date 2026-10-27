#include "benchmark.hpp"

namespace
{
    template <typename Multiply>
    void benchmarkToStream(const string& num1, const string& num2, Multiply multiply, const string& funcName, ostream& output)
    {
        auto start = chrono::high_resolution_clock::now();
        string result = multiply(num1, num2);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;

        output << "Function name: " << funcName << endl;
        output << "Num1: " << num1 << endl;
        output << "Num2: " << num2 << endl;
        output << "Result: " << result << endl;
        output << "Result length: " << result.length() << endl;
        output << "Execution time: " << duration.count() << " seconds" << endl;
        output << "----------------------------------------" << endl;
    }
}

void benchmark(string num1, string num2, string (*func)(string, string), const string& funcName, ofstream& output)
{
    benchmarkToStream(num1, num2, [func](const string& lhs, const string& rhs) {
        return func(lhs, rhs);
    }, funcName, output);
}

void benchmark(string num1, string num2, string (*func)(string, string), const string& funcName)
{
    benchmarkToStream(num1, num2, [func](const string& lhs, const string& rhs) {
        return func(lhs, rhs);
    }, funcName, cout);
}

void benchmark(string num1, string num2, string (*func)(string, string, int), const string& funcName, int cutoff, ofstream& output)
{
    benchmarkToStream(num1, num2, [func, cutoff](const string& lhs, const string& rhs) {
        return func(lhs, rhs, cutoff);
    }, funcName, output);
}

void benchmark(string num1, string num2, string (*func)(string, string, int), const string& funcName, int cutoff)
{
    benchmarkToStream(num1, num2, [func, cutoff](const string& lhs, const string& rhs) {
        return func(lhs, rhs, cutoff);
    }, funcName, cout);
}