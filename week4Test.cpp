#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <cctype>
#include "multiplySigned.hpp"
using namespace std;
using namespace std::chrono;
// Kiem tra chuoi co phai so nguyen duong hop le hay khong
bool isValidNumber(const string& number) 
{
    if (number.empty() || number[0] == '0')
        return false;

    for (char c : number) 
    {
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    }
    return true;
}
// Tinh phan du cua so nguyen rat lon ma khong can chuyen sang int
long long modulo(const string& number, long long p) 
{
    long long remainder = 0;
    for (char c : number) 
    {
        remainder = (remainder * 10 + (c - '0')) % p;
    }
    return remainder;
}
// Kiem tra tinh chat: (a * b) % p = ((a % p) * (b % p)) % p
bool verifyMultiplication(const string& a,const string& b,const string& result) 
{
    vector<long long> primes = {1000000007LL,1000000009LL,998244353LL};
    for (long long p : primes) 
    {
        long long aMod = modulo(a, p);
        long long bMod = modulo(b, p);
        long long resultMod = modulo(result, p);
        if ((aMod * bMod) % p != resultMod)
            return false;
    }
    return true;
}
int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        cout << "Usage: week4Test.exe <test-file>" << endl;
        return 1;
    }
    ifstream file(argv[1]);

    if (!file.is_open()) {
        cout << "Cannot open test file!" << endl;
        return 1;
    }
    string num1, num2;
    if (!getline(file, num1) || !getline(file, num2)) 
    {
        cout << "FAIL: Missing input number!" << endl;
        return 1;
    }
    if (!isValidNumber(num1) || !isValidNumber(num2)) 
    {
        cout << "FAIL: Invalid input!" << endl;
        return 1;
    }
    cout << "Digits: " << num1.length()<< " x " << num2.length() << endl;
    int cutoff = 64;
    auto start = high_resolution_clock::now();
    string result = multiplySignedKaratsuba(num1, num2, cutoff);
    auto end = high_resolution_clock::now();
    double seconds = duration<double>(end - start).count();
    cout << "Karatsuba cutoff: " << cutoff << endl;
    cout << "Result length: " << result.length() << endl;
    cout << "Execution time: " << seconds << " seconds" << endl;
    // Tich cua hai so n chu so co do dai n+m-1 hoac n+m
    size_t minLength = num1.length() + num2.length() - 1;
    size_t maxLength = num1.length() + num2.length();
    bool lengthValid = result.length() >= minLength &&result.length() <= maxLength;
    bool resultValid = isValidNumber(result);
    bool moduloValid = resultValid && verifyMultiplication(num1, num2, result);
    // Kiem tra ket qua chinh xac voi cac truong hop dac biet
    bool hasExpectedResult = false;
    string expectedResult;
    if (num1 == "1") 
    {
        expectedResult = num2;
        hasExpectedResult = true;
    }
    else if (num2 == "1") 
    {
        expectedResult = num1;
        hasExpectedResult = true;
    }
    else 
    {
        // Neu mot so co dang 1000...000 (luy thua cua 10)
        // thi tich bang so con lai va cac chu so 0 duoc them vao
        auto isPowerOfTen = [](const string& s) 
        {
            return s.length() > 1 &&s[0] == '1' &&s.find_first_not_of('0', 1) == string::npos;
        };
        if (isPowerOfTen(num1)) 
        {
            expectedResult = num2 + string(num1.length() - 1, '0');
            hasExpectedResult = true;
        }
        else if (isPowerOfTen(num2)) 
        {
            expectedResult = num1 + string(num2.length() - 1, '0');
            hasExpectedResult = true;
        }
    }
    if (hasExpectedResult) 
    {
        bool exactMatch = (result == expectedResult);
        cout << "Exact result comparison: "<< (exactMatch ? "PASS" : "FAIL") << endl;
        if (!exactMatch)
            return 1;
    }
    else {
        cout << "Exact result comparison: NOT APPLICABLE" << endl;
    }
    cout << "Length check: "<< (lengthValid ? "PASS" : "FAIL") << endl;
    cout << "Modulo verification: "<< (moduloValid ? "PASS" : "FAIL") << endl;
    // Chi so sanh Schoolbook voi Karatsuba khi input khong qua lon
    if (num1.length() <= 10000 && num2.length() <= 10000) {
        string schoolbookResult =multiplySignedSchoolbook(num1, num2);
        cout << "Schoolbook comparison: "<< (schoolbookResult == result ? "PASS" : "FAIL")<< endl;
        if (schoolbookResult != result)
            return 1;
    } else {
        cout << "Schoolbook comparison: SKIPPED (large input)" << endl;
    }
    if (!lengthValid || !moduloValid) 
    {
        cout << "Overall: FAIL" << endl;
        return 1;
    }
    cout << "Overall: PASS (checks completed)" << endl;
    return 0;
}
