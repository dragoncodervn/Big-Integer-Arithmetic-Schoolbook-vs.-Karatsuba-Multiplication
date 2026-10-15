#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <iomanip>
using namespace std;

string add(string num1, string num2); //cộng hai số ớn
int compareNumbers(const string& num1, const string& num2); //so sánh 2 số
string subtractPositive(const string& num1, const string& num2);//trừ hai số không âm khi số thứ nhất >= số thứ hai
string subtract(string num1, string num2);// trừ hai số
string multiply(string num1, string num2); // nhân bằng Schoolbook
string karatsuba(string num1, string num2);//Nhân bằng Karatsuba
void benchmark(string num1, string num2, string (*func)(string, string), const string& funcName, ofstream& output);//Đo thời gian thực thi
bool readInput(ifstream& input, string& num1, string& num2);//Đọc dữ liệu từ file test


int main(void)// void trong dấu ngoặc: hàm không nhận tham số.
{
    string num1, num2;
    ofstream output("output.txt");// ghi output ra file .txt
    //ktra file output
    if (!output.is_open())
    {
        cerr << "Cannot open output.txt" << endl;
        return 1; // kết thúc chương trình với mã báo lỗi
    }

    string folderName = "test\\"; //folderName lưu tên thư mục chứa các file test.
    string testFiles[] = 
    {
        "test100digits.txt", //testFiles[0]
        "test10000digits.txt",
        "test100000digits.txt",
        "test3digits.txt",
        "testDuplicateValues.txt",
        "testEmptyInput.txt",
        "testSingleElement.txt",
        "testDuplicateDigitsValues.txt"
    };
    // duyệt từng file test
    for(int i = 0; i < sizeof(testFiles) / sizeof(testFiles[0]); ++i)  //for (int i = 0; i < 8; i++)
    {
       // dùng để đọc dữ liệu từ file 
       // vd: i=0
       //folderName = "test\"
       //testFiles[0] = "test100digits.txt"
       // => test\test100digits.txt  => chương trình sẽ cố gắng mở file này để đọc
        ifstream input(folderName + testFiles[i]);
        //// Nếu đọc được hai số thì chạy cả hai thuật toán
        if (readInput(input, num1, num2)) // nhận file đang mở, đọc hai dòng và lưu vào num1, num2  => nó trả về bool
        {
            // nếu true, gọi benchmark() hai lần
            benchmark(num1, num2, &multiply, "multiply", output); // Đo thời gian nhân bằng Schoolbook
            benchmark(num1, num2, &karatsuba, "karatsuba", output); // Đo thời gian nhân bằng Karatsuba
        }
        else
        {
            output << "Skipping test file: " << testFiles[i] << " due to input errors." << endl;
            output << "----------------------------------------" << endl;
        }
        input.close(); // đóng file input sau khi xử lý xong
    }
    output.close(); // đóng file kết quả (output)
    return 0;
}
//CỘNG HAI SỐ NGUYÊN LỚN (CỘNG TỪNG CHỮ SỐ TỪ PHẢI SANG TRÁI)
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

void benchmark(string num1, string num2, string (*func)(string, string), const string& funcName, ofstream& output)
{
    string result; // Call the function once to get the result
    auto start = chrono::high_resolution_clock::now();
    result = func(num1, num2);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    output << "Function name: " << funcName << endl;
    output << "Num1: " << num1 << endl;
    output << "Num2: " << num2 << endl;
    output << "Result: " << result << endl;
    output << "Result length: " << result.length() << endl;
    output << "Execution time: " << duration.count() << " seconds" << endl;
    output << "----------------------------------------" << endl;
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

    return true;
}