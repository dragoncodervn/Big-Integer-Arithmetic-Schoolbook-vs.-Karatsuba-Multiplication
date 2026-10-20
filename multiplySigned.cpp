#include "multiplySigned.hpp"
#include "schoolbookMultiply.hpp"
#include "karatsuba.hpp"

//HÀM XỬ LÝ DẤU ÂM
string multiplySigned(string num1,string num2,string (*func)(string, string))
{
//string (*func)(string, string) - con trỏ hàm :cho phép truyền một hàm khác vào multiplySigned()
    // Kiểm tra số thứ nhất có dấu âm không
    bool negative1 = !num1.empty() && num1[0] == '-';

    // Kiểm tra số thứ hai có dấu âm không
    bool negative2 = !num2.empty() && num2[0] == '-';

    // Nếu có dấu âm thì xóa dấu âm ở đầu chuỗi
    if (negative1)
        num1.erase(0, 1);
//string.erase(start, length);
    if (negative2)
        num2.erase(0, 1);

    // Nhân hai giá trị không âm bằng thuật toán được truyền vào
    string result = func(num1, num2);

    // Nếu kết quả bằng 0 thì không thêm dấu âm
    if (result == "0")
        return "0";

    // Nếu đúng một số âm thì kết quả âm
    if (negative1 != negative2)
        result = "-" + result;

    return result;
}
    // Hàm trung gian để benchmark gọi Schoolbook có xử lý dấu
    string multiplySignedSchoolbook(string num1, string num2)
    {
        return multiplySigned(num1, num2, &multiply);
    }

    // Hàm trung gian để benchmark gọi Karatsuba có xử lý dấu
    string multiplySignedKaratsuba(string num1, string num2)
    {
        return multiplySigned(num1, num2, &karatsuba);
    }