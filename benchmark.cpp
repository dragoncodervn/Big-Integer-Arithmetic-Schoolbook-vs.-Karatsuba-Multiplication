#include "benchmark.hpp"

void benchmark(string num1, string num2, string (*func)(string, string), const string& funcName, ofstream& output)
{
    string result; // Call the function once to get the result
    auto start = chrono::high_resolution_clock::now();
    result = func(num1, num2);
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

void benchmark(string num1, string num2, string (*func)(string, string), const string& funcName)
{
    string result; // Call the function once to get the result
    auto start = chrono::high_resolution_clock::now();
    result = func(num1, num2);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Function name: " << funcName << endl;
    cout << "Num1: " << num1 << endl;
    cout << "Num2: " << num2 << endl;
    cout << "Result: " << result << endl;
    cout << "Result length: " << result.length() << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;
    cout << "----------------------------------------" << endl;
}