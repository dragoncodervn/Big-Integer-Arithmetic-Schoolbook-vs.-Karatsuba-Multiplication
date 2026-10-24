
#include <iostream>
#include <fstream>
#include <string>
#include <random>
using namespace std;

// Tạo một số nguyên dương có đúng n chữ số
string generateNumber(size_t n, mt19937& rng)
{
    uniform_int_distribution<int> firstDigit(1, 9);
    uniform_int_distribution<int> digit(0, 9);

    string number;
    number.reserve(n);

    // Chữ số đầu tiên không được bằng 0
    number += char('0' + firstDigit(rng));

    // Sinh ngẫu nhiên các chữ số còn lại
    for (size_t i = 1; i < n; i++)
    {
        number += char('0' + digit(rng));
    }
    return number;
}

int main()
{
    // Dùng seed cố định để có thể tái tạo bộ test
    mt19937 rng(2026);

    // Các kích thước này vẫn đủ lớn để benchmark nhưng không vượt giới hạn
    // bộ nhớ thực tế. 10^10 chữ số đã cần khoảng 20 GB cho một file gồm
    // hai số, chưa tính bộ nhớ tạm và thời gian xử lý.
    const size_t sizes[] = {1000, 5000, 20000, 1000000};

    for (size_t n : sizes)
    {
        string filename = "test/test" + to_string(n) + "digits.txt";
        ofstream output(filename);

        if (!output.is_open())
        {
            cerr << "Cannot create: " << filename << endl;
            continue;
        }

        // Mỗi file chứa hai số nguyên, mỗi số một dòng
        output << generateNumber(n, rng) << '\n';
        output << generateNumber(n, rng) << '\n';

        output.close();

        cout << "Created: " << filename<< " (" << n << " digits per number)" << endl;
    }
    return 0;
}
