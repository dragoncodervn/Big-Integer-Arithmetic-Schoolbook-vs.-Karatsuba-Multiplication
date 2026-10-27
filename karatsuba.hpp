#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

string add(string num1, string num2); //cộng hai số ớn
string subtractPositive(const string& num1, const string& num2);//trừ hai số không âm khi số thứ nhất >= số thứ hai
string subtract(string num1, string num2);// trừ hai số
string multiply(string num1, string num2); // nhân bằng Schoolbook
string karatsuba(string num1, string num2, int cutoff);//Nhân bằng Karatsuba