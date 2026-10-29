#include "handleInput.hpp"
#include "benchmark.hpp"
#include "multiplySigned.hpp"

int main(int argc, char **argv)
{
    vector<size_t> cutoffs = {2, 4, 8, 16, 24, 32, 48, 64, 96, 128, 192, 256, 512};

    if (argc == 3)
    {
        string num1, num2;
        num1 = argv[1];
        num2 = argv[2];
        if (readInput(num1, num2)) // kiểm tra và chuẩn hóa hai tham số
        {
            benchmark(num1, num2, &multiplySignedSchoolbook, "multiply"); // Đo thời gian nhân bằng Schoolbook
            for (size_t cutoff : cutoffs)
            {
                cout << "Cutoff: " << cutoff << endl;
                benchmark(num1, num2, &multiplySignedKaratsuba, "karatsuba", cutoff); // Đo thời gian nhân bằng Karatsuba
            }
        }
        else
        {
            cerr << "Invalid input. Please enter two valid numbers." << endl;
        }
    }
    else if (argc == 1)
    {
        string num1, num2;
        ofstream output("output.txt"); // ghi output ra file .txt
        // ktra file output
        if (!output.is_open())
        {
            cerr << "Cannot open output.txt" << endl;
            return 1; // kết thúc chương trình với mã báo lỗi
        }

        string folderName = "test\\"; // folderName lưu tên thư mục chứa các file test.
        string testFiles[] =
{
    // Week 1
    "test3digits.txt",
    "test100digits.txt",
    "testDuplicateValues.txt",
    "testEmptyInput.txt",
    "testSingleElement.txt",
    "testDuplicateDigitsValues.txt",

    // Week 2 - Negative numbers
    "testNegativePositive.txt",
    "testPositiveNegative.txt",
    "testNegativeNegative.txt",
    "testNegativeZero.txt",

    // Week 2 - Input validation
    "testLeadingZeros.txt",
    "testInvalidCharacters.txt",
    "testInvalidSign.txt",

    // Week 3 - Large integer benchmark
    "test1000digits.txt",
    "test5000digits.txt",
    "test10000digits.txt",
    "test20000digits.txt",
    "test100000digits.txt",
    "test200000digits.txt",
    "test300000digits.txt",
    "test500000digits.txt",
    "test750000digits.txt",
    "test1000000digits.txt",
    "test1000000Identity.txt",
    "test100000Identity.txt",
    "test200000PowerOfTen.txt",
    
};
        // duyệt từng file test
        for (int i = 0; i < sizeof(testFiles) / sizeof(testFiles[0]); ++i) // for (int i = 0; i < 8; i++)
        {
            // dùng để đọc dữ liệu từ file
            // vd: i=0
            // folderName = "test\"
            // testFiles[0] = "test100digits.txt"
            // => test\test100digits.txt  => chương trình sẽ cố gắng mở file này để đọc
            ifstream input(folderName + testFiles[i]);
            //// Nếu đọc được hai số thì chạy cả hai thuật toán
            if (readInput(input, num1, num2)) // nhận file đang mở, đọc hai dòng và lưu vào num1, num2  => nó trả về bool
            {
                // nếu true, gọi benchmark() hai lần
                benchmark(num1, num2, &multiplySignedSchoolbook, "multiply", output); // Đo thời gian nhân bằng Schoolbook
                for (size_t cutoff : cutoffs)
                {
                    output << "Cutoff: " << cutoff << endl;
                    benchmark(num1, num2, &multiplySignedKaratsuba, "karatsuba", cutoff, output); // Đo thời gian nhân bằng Karatsuba
                }
            }
            else
            {
                output << "Skipping test file: " << testFiles[i] << " due to input errors." << endl;
                output << "----------------------------------------" << endl;
            }
            input.close(); // đóng file input sau khi xử lý xong
        }
        output.close(); // đóng file kết quả (output)
    }
    else if (argc > 3)
    {
        cerr << "Invalid number of arguments. Please provide two numbers as command line arguments." << endl;
        return 1; // kết thúc chương trình với mã báo lỗi
    }
    return 0;
}
