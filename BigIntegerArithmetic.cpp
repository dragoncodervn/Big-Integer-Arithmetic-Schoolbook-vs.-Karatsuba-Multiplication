#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <iomanip>
using namespace std;

string add(string num1, string num2);
int compareNumbers(const string& num1, const string& num2);
string subtractPositive(const string& num1, const string& num2);
string subtract(string num1, string num2);
string multiply(string num1, string num2);
string karatsuba(string num1, string num2);
void benchmark(string num1, string num2, string (*func)(string, string), const string& funcName, ofstream& output);
bool readInput(ifstream& input, string& num1, string& num2);


int main(void)
{
    string num1, num2;
    ofstream output("output.txt");
    if (!output.is_open())
    {
        cerr << "Cannot open output.txt" << endl;
        return 1;
    }

    string folderName = "test\\";
    string testFiles[] = {
        "test100digits.txt",
        "test10000digits.txt",
        "test100000digits.txt",
        "test3digits.txt",
        "testDuplicateValues.txt",
        "testEmptyInput.txt",
        "testSingleElement.txt",
        "testDuplicateDigitsValues.txt"
    };

    for(int i = 0; i < sizeof(testFiles) / sizeof(testFiles[0]); ++i)
    {
        ifstream input(folderName + testFiles[i]);
        if (readInput(input, num1, num2))
        {
            benchmark(num1, num2, &multiply, "multiply", output);
            benchmark(num1, num2, &karatsuba, "karatsuba", output);
        }
        else
        {
            output << "Skipping test file: " << testFiles[i] << " due to input errors." << endl;
            output << "----------------------------------------" << endl;
        }
        input.close();
    }

    output.close();
    return 0;
}

string add(string num1, string num2)
{
    string result = "";
    int carry = 0;
    int i = num1.length() - 1;
    int j = num2.length() - 1;

    while (i >= 0 || j >= 0 || carry)
    {
        int sum = carry;
        if (i >= 0)
            sum += num1[i--] - '0';
        if (j >= 0)
            sum += num2[j--] - '0';
        carry = sum / 10;
        result.insert(result.begin(), (sum % 10) + '0');
    }
    return result;
}

// Functions to subtract two numbers represented as strings
// Function to compare two numbers represented as strings
int compareNumbers(const string& num1, const string& num2)
{
    if (num1.length() != num2.length())
        return num1.length() < num2.length() ? -1 : 1;
    if (num1 == num2)
        return 0;
    return num1 < num2 ? -1 : 1;
}

string subtractPositive(const string& num1, const string& num2)
{
    string result;
    int borrow = 0;
    int i = num1.length() - 1;
    int j = num2.length() - 1;

    while (i >= 0)
    {
        int diff = (num1[i] - '0') - borrow;
        if (j >= 0)
            diff -= (num2[j--] - '0');
        if (diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        result.insert(result.begin(), diff + '0');
        i--;
    }

    while (result.length() > 1 && result[0] == '0')
        result.erase(result.begin());

    return result;
}

string subtract(string num1, string num2)
{
    if (compareNumbers(num1, num2) == 0)
        return "0";
    if (compareNumbers(num1, num2) > 0)
        return subtractPositive(num1, num2);
    return "-" + subtractPositive(num2, num1);
}
//------------------------------------------------------------------------------

string multiply(string num1, string num2)
{
    if (num1 == "0" || num2 == "0")
        return "0";

    vector<int> result(num1.length() + num2.length(), 0);

    for (int i = num1.length() - 1; i >= 0; i--)
    {
        for (int j = num2.length() - 1; j >= 0; j--)
        {
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int sum = mul + result[i + j + 1];

            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }

    string product = "";
    for (int num : result)
    {
        if (!(product.empty() && num == 0))
            product += to_string(num);
    }

    return product.empty() ? "0" : product;
}

string karatsuba(string num1, string num2)
{
    if (num1.empty() || num2.empty() || num1 == "0" || num2 == "0")
        return "0";
    if (num1.length() <= 32 || num2.length() <= 32)
        return multiply(num1, num2);

    size_t maxLength = max(num1.length(), num2.length());
    if (num1.length() < maxLength)
        num1 = string(maxLength - num1.length(), '0') + num1;
    if (num2.length() < maxLength)
        num2 = string(maxLength - num2.length(), '0') + num2;
    size_t halfLength = maxLength / 2;

    // Split the digit sequences in the middle
    string high1 = num1.substr(0, num1.length() - halfLength);
    string low1 = num1.substr(num1.length() - halfLength);
    string high2 = num2.substr(0, num2.length() - halfLength);
    string low2 = num2.substr(num2.length() - halfLength);

    // 3 recursive calls

    string p1 = karatsuba(high1, high2);
    string p2 = karatsuba(low1, low2);
    string p3 = karatsuba(add(low1, high1), add(low2, high2));
    string mid = subtract(subtract(p3, p2), p1);

    // Combine the results
    string result = add(add((p1 + string(2 * halfLength, '0')),(mid + string(halfLength, '0'))),p3);

    return result;
}

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

bool readInput(ifstream& input, string& num1, string& num2)
{
    if (!input.is_open())
    {
        cerr << "Cannot open input file. Current working directory must contain the test files." << endl;
        return false;
    }

    if (!getline(input, num1) || !getline(input, num2) ||
        num1.empty() || num2.empty())
    {
        cerr << "Input file must contain two non-empty lines." << endl;
        return false;
    }

    return true;
}