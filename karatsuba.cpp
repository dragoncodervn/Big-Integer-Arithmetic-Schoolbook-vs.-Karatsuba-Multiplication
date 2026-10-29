#include "karatsuba.hpp"
#include "handleInput.hpp"
#include <stdexcept>

namespace
{
    string normalizeUnsigned(const string& num)
    {
        size_t firstDigit = num.find_first_not_of('0');
        return firstDigit == string::npos ? "0" : num.substr(firstDigit);
    }
}

string add(string num1, string num2)
{
    num1 = normalizeUnsigned(num1);
    num2 = normalizeUnsigned(num2);

    string result; // lưu các chữ số theo thứ tự từ hàng đơn vị lên
    result.reserve(max(num1.length(), num2.length()) + 1);
    int carry = 0; // số nhớ khi cộng
    //bắt đầu từ chữ số cuối cùng (hàng đơn vị)
    int i = num1.length() - 1; 
    int j = num2.length() - 1;
   // tiếp tục vòng lặp khi còn ít nhất một trong 3 điều kiện:
   //1. num1 vẫn còn chữ số
   //2. num2 vẫn còn chữ số
   //3. Vẫn còn số nhớ (carry khác 0)
    while (i >= 0 || j >= 0 || carry)
    {
        int sum = carry; // cộng thêm số nhớ từ bước trước
        if (i >= 0)
            sum += num1[i--] - '0';
        //num1[i--] nghĩa là sử dụng ký tự tại vị trí i trước, rồi giảm i đi 1.
        //- '0' chuyển ký tự chữ số thành giá trị số nguyên. vd: '9' - '0' // 9
        if (j >= 0)
            sum += num2[j--] - '0';
        carry = sum / 10; // lấy số nhớ
        result.push_back((sum % 10) + '0');
        //sum % 10 lấy chữ số hàng đơn vị. Ví dụ 15 % 10 = 5.
        //push_back thêm vào cuối với chi phí O(1); chuỗi được đảo ngược một lần sau vòng lặp
    }
    reverse(result.begin(), result.end());
    return normalizeUnsigned(result);
}


string subtractPositive(const string& num1, const string& num2) // trừ từng chữ số 
{
    string normalizedNum1 = normalizeUnsigned(num1);
    string normalizedNum2 = normalizeUnsigned(num2);
    if (compareNumbers(normalizedNum1, normalizedNum2) < 0)
        throw invalid_argument("subtractPositive requires the first operand to be greater than or equal to the second.");

    string result;
    result.reserve(normalizedNum1.length());
    int borrow = 0;
    int i = static_cast<int>(normalizedNum1.length()) - 1;
    int j = static_cast<int>(normalizedNum2.length()) - 1;

    while (i >= 0)
    {
        int diff = (normalizedNum1[i] - '0') - borrow;//Lấy chữ số hiện tại của num1, chuyển thành số nguyên rồi trừ đi số mượn.
        if (j >= 0) // nếu num2 còn chữ số thì tiếp tục trừ
            diff -= (normalizedNum2[j--] - '0');
        
        if (diff < 0) // nếu hiệu âm thì phải mượn 1 từ hàng bên trái
        {
            diff += 10;
            borrow = 1;
        }
        //vd: Khi tính 123 − 45, tại bước đầu tiên diff = 3 − 5 = −2
        // chương trình sẽ : cộng diff với 10 và đặt borrow=1
        else
        {
            borrow = 0; // khôgn cần mượn nữa
        }
        result.push_back(diff + '0');// Thêm chữ số vừa tính vào cuối (O(1)); đảo ngược một lần sau vòng lặp
        i--;// chuyển sang chữ số tiếp theo bên trái
    }

    if (borrow != 0)
        throw logic_error("subtractPositive ended with an unhandled borrow.");
    reverse(result.begin(), result.end());
    // normalizeUnsigned xóa các số 0 thừa ở đầu trong một lượt duyệt, nhưng vẫn giữ lại một chữ số
    return normalizeUnsigned(result);
}
// So sánh hai số không âm, quyết định thứ tự trừ và dấu của kết quả
// subtractPositive("123", "45"); // "78"
// subtract("123", "45"); // "78"
// subtract("45", "123"); // "-78"
string subtract(string num1, string num2) // // Trừ hai số không âm và xác định dấu kết quả
{
    num1 = normalizeUnsigned(num1);
    num2 = normalizeUnsigned(num2);

    // Hai số bằng nhau thì hiệu bằng 0
    if (compareNumbers(num1, num2) == 0) 
        return "0";
    // Nếu num1 lớn hơn thì trừ trực tiếp
    if (compareNumbers(num1, num2) > 0)
        return subtractPositive(num1, num2);
    //// Nếu num1 nhỏ hơn thì đổi thứ tự trừ rồi thêm dấu âm
    string difference = subtractPositive(num2, num1);
    return difference == "0" ? "0" : "-" + difference;
}
//subtractPositive() thực hiện phép trừ từng chữ số bằng cách xử lý borrow
//subtract() dùng compareNumbers() để xác định thứ tự phép trừ và dấu kết quả

//------------------------------------------------------------------------------
//Schoolbook Multiplication
//vd: 123 x 45 = 5535
//    123
//   x 45
// ---------
//    615
//   4920
// ----------
//   5535  //Ta nhân từng chữ số của số thứ hai với số thứ nhất, rồi cộng các tích theo đúng hàng.

string karatsuba(string num1, string num2, int cutoff)
{
    if (cutoff <= 0)
        throw invalid_argument("Karatsuba cutoff must be greater than zero.");

    num1 = normalizeUnsigned(num1);
    num2 = normalizeUnsigned(num2);
    if (num1 == "0" || num2 == "0")
        return "0";
    //base case
    // Với số nhỏ hơn hoặc bằng cutoff chữ số, dùng Schoolbook
    if (num1.length() <= cutoff || num2.length() <= cutoff)
        return multiply(num1, num2);
    //Tìm độ dài lớn nhất của hai số
    size_t maxLength = max(num1.length(), num2.length());
    // Thêm số 0 vào đầu để hai chuỗi có cùng độ dài
    if (num1.length() < maxLength)
        num1 = string(maxLength - num1.length(), '0') + num1;
    if (num2.length() < maxLength)
        num2 = string(maxLength - num2.length(), '0') + num2;
    //// Số chữ số của phần thấp khi chia đôi
    size_t halfLength = maxLength / 2;

    // Chia num1 thành phần cao và phần thấp
    string high1 = num1.substr(0, num1.length() - halfLength);
    string low1 = num1.substr(num1.length() - halfLength);
    // Chia num2 thành phần cao và phần thấp
    string high2 = num2.substr(0, num2.length() - halfLength);
    string low2 = num2.substr(num2.length() - halfLength);

    // 3 recursive calls

    string p1 = karatsuba(high1, high2, cutoff);// high1 * high2
    string p2 = karatsuba(low1, low2, cutoff);// low1 * low2
    string p3 = karatsuba(add(low1, high1), add(low2, high2), cutoff); // (high1 + low1) * (high2 + low2)
    string mid = subtract(subtract(p3, p2), p1);// Tính phần giữa: mid = p3 - p2 - p1
    
    // Ghép kết quả theo các hàng của phép nhân 
    //theo công thức Karatsuba:p1 * 10^(2m) + mid * 10^m + p2
    string result = add(add((p1 + string(2 * halfLength, '0')),(mid + string(halfLength, '0'))),p2);

    return result;
}