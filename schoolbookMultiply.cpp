#include "schoolbookMultiply.hpp"

string multiply(string num1, string num2)
{
    if (num1 == "0" || num2 == "0")
        return "0";
    // Tạo vector lưu từng chữ số của kết quả
    // kết quả có tối đa num1.length() + num2.length() chữ số
    vector<int> result(num1.length() + num2.length(), 0);
    //duyệt từng chữ số của num1 từ phải sang trái
    for (int i = num1.length() - 1; i >= 0; i--)
    {
        //// Với mỗi chữ số của num1, nhân lần lượt với các chữ số num2
        for (int j = num2.length() - 1; j >= 0; j--)
        {
             // Chuyển hai ký tự thành số rồi thực hiện phép nhân
            int mul = (num1[i] - '0') * (num2[j] - '0'); // nhân hai chữ số hiện tại
            int sum = mul + result[i + j + 1]; // cộng tích vừa tính với giá trị đang có ở vị trí tương ứng

            result[i + j + 1] = sum % 10;// lưu hàng đơn vị vào vị trí tương ứng
            result[i + j] += sum / 10; // cộng phần nhớ vào vị trí bên trái
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


