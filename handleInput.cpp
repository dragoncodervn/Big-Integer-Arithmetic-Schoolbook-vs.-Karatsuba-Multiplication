#include "handleInput.hpp"

// Functions to subtract two numbers represented as strings
// Function to compare two numbers represented as strings
//SO SÁNH HAI SỐ NGUYÊN LỚN KHÔNG ÂM DƯỚI DẠNG CHUỖI
//Trả về -1 nếu num1 nhỏ hơn, 0 nếu bằng, 1 nếu lớn hơn
int compareNumbers(const string& num1, const string& num2)
{
    //// Số nào có nhiều chữ số hơn thì lớn hơn
    if (num1.length() != num2.length())
        return num1.length() < num2.length() ? -1 : 1;
//condition ? valueIfTrue : valueIfFalse;
    //// Nếu hai chuỗi giống nhau thì hai số bằng nhau
    if (num1 == num2)
        return 0;
    //// Nếu cùng độ dài thì so sánh từ trái sang phải
    return num1 < num2 ? -1 : 1;
}
/*
Tại sao cần hàm so sánh? Trong Karatsuba, chương trình cần thực hiện các phép trừ trung gian
Hàm subtract() dùng compareNumbers() để quyết định số nào lớn hơn, từ đó xác định thứ tự trừ và dấu kết quả.
*/
//subtractPositive(num1, num2) :Thực hiện phép trừ từng chữ số, giả định num1 >= num2 và cả hai không âm
bool normalizeNumber(string& num)
{
    // Không chấp nhận chuỗi rỗng
    if (num.empty())
        return false;

    // Kiểm tra số có dấu âm hay không
    bool negative = (num[0] == '-');

    // Nếu có dấu âm thì bắt đầu kiểm tra từ vị trí 1
    int start = negative ? 1 : 0;

    // Trường hợp chỉ có dấu "-" mà không có chữ số
    if (start == num.length())
        return false;

    // Kiểm tra từng ký tự có phải chữ số từ 0 đến 9 không
    for (int i = start; i < num.length(); i++)
    {
        if (num[i] < '0' || num[i] > '9')
            return false;
    }

    // Xóa dấu âm tạm thời để xử lý phần chữ số
    if (negative)
        num.erase(0, 1);

    // Xóa các số 0 thừa ở đầu, nhưng giữ lại ít nhất 1 chữ số
    while (num.length() > 1 && num[0] == '0')
        num.erase(0, 1);

    // Nếu số khác 0 và ban đầu có dấu âm thì thêm dấu âm trở lại
    if (negative && num != "0")
        num = "-" + num;

    return true;
}



bool readInput(ifstream& input, string& num1, string& num2)
{
//ifstream& input : tham chiếu đến file đang đọc
//string& num1 : tham chiếu đến biến lưu số thứ nhất
// Nghĩa là khi readInput() gán giá trị cho num1, num2, 
// hai biến tương ứng trong main() cũng được cập nhật

    if (!input.is_open()) // ktra file có mở được không
    {
        cerr << "Cannot open input file. Current working directory must contain the test files." << endl;
        return false;
    }

    if (!getline(input, num1) || !getline(input, num2) || num1.empty() || num2.empty()) 
    //!getline(input, num1) : không đọc được dòng thứ nhất
    //!getline(input, num2) : không đọc được dòng thứ hai
    //num1.empty() : chuỗi num1 rỗng
    {
        cerr << "Input file must contain two non-empty lines." << endl;
        return false;
    }
    // Kiểm tra và chuẩn hóa cả hai số
    if (!normalizeNumber(num1) || !normalizeNumber(num2))
    {
        cerr << "Invalid integer input." << endl;
        return false;
    }

    return true;
}

bool readInput(string& num1, string& num2)
{
    if (num1.empty() || num2.empty()) 
    {
        cerr << "Input must contain two numbers or none." << endl;
        return false;
    }
    // Kiểm tra và chuẩn hóa cả hai số
    if (!normalizeNumber(num1) || !normalizeNumber(num2))
    {
        cerr << "Invalid integer input." << endl;
        return false;
    }
    return true;
}
