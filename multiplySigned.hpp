#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "schoolbookMultiply.hpp"
#include "karatsuba.hpp"
using namespace std;

string multiplySigned(string num1, string num2, string (*func)(string, string));
string multiplySigned(string num1, string num2, string (*func)(string, string, int), int cutoff);
string multiplySignedSchoolbook(string num1, string num2);
string multiplySignedKaratsuba(string num1, string num2, int cutoff);