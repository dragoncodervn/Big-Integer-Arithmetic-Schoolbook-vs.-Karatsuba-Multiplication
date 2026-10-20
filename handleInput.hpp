#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

int compareNumbers(const string& num1, const string& num2); //so sánh 2 số
bool readInput(ifstream& input, string& num1, string& num2);//Đọc dữ liệu từ file test
bool readInput(string& num1, string& num2);//Đọc dữ liệu từ màn hình console
bool normalizeNumber(string& num);