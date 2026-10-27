#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <iomanip>
using namespace std;

void benchmark(string num1, string num2, string (*func)(string, string), const string& funcName, ofstream& output);//Đo thời gian thực thi
void benchmark(string num1, string num2, string (*func)(string, string), const string& funcName);//Đo thời gian thực thi và hiển thị kết quả ra màn hình
void benchmark(string num1, string num2, string (*func)(string, string, int), const string& funcName, int cutoff, ofstream& output);//Đo thời gian thực thi Karatsuba
void benchmark(string num1, string num2, string (*func)(string, string, int), const string& funcName, int cutoff);//Đo thời gian thực thi Karatsuba và hiển thị kết quả ra màn hình