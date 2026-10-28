#include <iostream>
#include <fstream>
#include <string>
#include <random>
using namespace std;

// Tao mot so nguyen lon theo loai du lieu duoc chon
string generateNumber(int n, int type, mt19937 &rng) 
{
    string number;
    number.reserve(n);
    uniform_int_distribution<int> digit(0, 9);
    uniform_int_distribution<int> firstDigit(1, 9);

    for (int i = 0; i < n; i++) 
    {
        char c;
        if (type == 1) 
        {
            // Sinh chu so ngau nhien
            c = '0' + (i == 0 ? firstDigit(rng) : digit(rng));
        }
        else if (type == 2) 
        {
            // Lap lai chu so 9 va 8
            c = (i % 2 == 0) ? '9' : '8';
        }
        else if (type == 3) 
        {
            // Phan lon cac chu so la 0
            c = (i % 10 == 0) ? '7' : '0';
        }
        else {
            c = '1';
        }
        number += c;
    }

    return number;
}

// Ghi hai so nguyen vao file test
void createTest(int n, int type, const string &filename, mt19937 &rng) 
{
    ofstream file("test/" + filename);
    if (!file) 
    {
        cout << "Cannot create " << filename << endl;
        return;
    }
    file << generateNumber(n, type, rng) << '\n';
    file << generateNumber(n, type, rng) << '\n';
    cout << "Created: " << filename << endl;
}

int main() 
{
    //dùng seed cố định để tái tạo dữ liệu
    mt19937 rng(2026);

    createTest(200000, 1, "test200000digits.txt", rng);
    createTest(300000, 2, "test300000digits.txt", rng);
    createTest(500000, 3, "test500000digits.txt", rng);
    createTest(750000, 1, "test750000digits.txt", rng);
    return 0;
}
