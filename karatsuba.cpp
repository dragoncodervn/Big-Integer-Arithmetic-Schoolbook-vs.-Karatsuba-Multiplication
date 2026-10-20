#include "karatsuba.hpp"
#include "handleInput.hpp"

string add(string num1, string num2)
{
    string result = ""; // chuỗi rỗng để lưu kết quả phép cộng
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
        result.insert(result.begin(), (sum % 10) + '0');
        //lấy hàng đơn vị rồi chèn vào đầu kết quả
        //sum % 10 lấy chữ số hàng đơn vị. Ví dụ 15 % 10 = 5.
        //insert(result.begin(), ...) chèn chữ số vào đầu chuỗi kết quả.; chèn vào đầu vì ta đang tính từ phải sang trái
    }
    return result;
}


string subtractPositive(const string& num1, const string& num2) // trừ từng chữ số 
{
    string result;
    int borrow = 0;
    int i = num1.length() - 1;
    int j = num2.length() - 1;

    while (i >= 0)
    {
        int diff = (num1[i] - '0') - borrow;//Lấy chữ số hiện tại của num1, chuyển thành số nguyên rồi trừ đi số mượn.
        if (j >= 0) // nếu num2 còn chữ số thì tiếp tục trừ
            diff -= (num2[j--] - '0');
        
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
        result.insert(result.begin(), diff + '0');// Chèn chữ số vừa tính vào đầu chuỗi kết quả
        i--;// chuyển sang chữ số tiếp theo bên trái
    }
    // xóa các số 0 thừa ở đầu, nhưng vẫn giữ lại một chữ số
    while (result.length() > 1 && result[0] == '0')
        result.erase(result.begin());

    return result;
}
// So sánh hai số không âm, quyết định thứ tự trừ và dấu của kết quả
// subtractPositive("123", "45"); // "78"
// subtract("123", "45"); // "78"
// subtract("45", "123"); // "-78"
string subtract(string num1, string num2) // // Trừ hai số không âm và xác định dấu kết quả
{
    // Hai số bằng nhau thì hiệu bằng 0
    if (compareNumbers(num1, num2) == 0) 
        return "0";
    // Nếu num1 lớn hơn thì trừ trực tiếp
    if (compareNumbers(num1, num2) > 0)
        return subtractPositive(num1, num2);
    //// Nếu num1 nhỏ hơn thì đổi thứ tự trừ rồi thêm dấu âm
    return "-" + subtractPositive(num2, num1);
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

string karatsuba(string num1, string num2)
{
    if (num1.empty() || num2.empty() || num1 == "0" || num2 == "0")
        return "0";
    //base case
    // Với số nhỏ (<= 32 chữ số), dùng Schoolbook
    if (num1.length() <= 32 || num2.length() <= 32)
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

    string p1 = karatsuba(high1, high2);// high1 * high2
    string p2 = karatsuba(low1, low2);// low1 * low2
    string p3 = karatsuba(add(low1, high1), add(low2, high2)); // (high1 + low1) * (high2 + low2)
    string mid = subtract(subtract(p3, p2), p1);// Tính phần giữa: mid = p3 - p2 - p1
    
    // Ghép kết quả theo các hàng của phép nhân 
    //theo công thức Karatsuba:p1 * 10^(2m) + mid * 10^m + p2
    string result = add(add((p1 + string(2 * halfLength, '0')),(mid + string(halfLength, '0'))),p2);

    return result;
}