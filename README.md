# Big Integer Arithmetic

So sánh phép nhân số nguyên rất lớn bằng hai thuật toán:

- **Schoolbook multiplication**: nhân từng cặp chữ số, độ phức tạp `O(n^2)`.
- **Karatsuba multiplication**: chia để trị, giảm số phép nhân cần thực hiện và có độ phức tạp lý thuyết khoảng `O(n^log2(3))`.

Chương trình biểu diễn số bằng `std::string`, vì vậy không phụ thuộc vào giới hạn của các kiểu số nguyên dựng sẵn. Chương trình hỗ trợ số dương, số âm, số 0, số có các chữ số 0 ở đầu và kiểm tra ký tự đầu vào không hợp lệ.

## Cấu trúc chính

| Tệp | Chức năng |
|---|---|
| `BigIntegerArithmetic.cpp` | Điểm bắt đầu chương trình, chạy các test và ghi kết quả |
| `handleInput.cpp/.hpp` | Đọc, kiểm tra và chuẩn hóa dữ liệu đầu vào |
| `schoolbookMultiply.cpp/.hpp` | Cài đặt phép nhân Schoolbook |
| `karatsuba.cpp/.hpp` | Cài đặt phép cộng, phép trừ và phép nhân Karatsuba |
| `multiplySigned.cpp/.hpp` | Xử lý dấu trước và sau khi nhân |
| `benchmark.cpp/.hpp` | Đo thời gian chạy và in kết quả |
| `test/` | Các tệp dữ liệu kiểm thử |
| `output.txt` | Kết quả benchmark khi chạy chế độ kiểm thử tự động |

## Yêu cầu

- Trình biên dịch C++ có hỗ trợ C++11 trở lên.
- `g++` hoặc MinGW trên Windows.

## Biên dịch

Mở Terminal/Command Prompt tại thư mục gốc của dự án và chạy:

```cmd
g++ BigIntegerArithmetic.cpp benchmark.cpp handleInput.cpp karatsuba.cpp multiplySigned.cpp schoolbookMultiply.cpp -o BigIntegerArithmetic
```
OR
```cmd
g++ *cpp -o BigIntegerArithmetic
```

## Chạy chương trình

### Chạy toàn bộ test có sẵn

Chạy từ thư mục gốc của dự án:

Windows:
```cmd
BigIntegerArithmetic.exe
```

Linux:
```terminal
./BigIntegerArithmetic.exe
```

Chương trình sẽ:

1. Đọc lần lượt các tệp trong thư mục `test\`.
2. Bỏ qua các tệp có dữ liệu rỗng hoặc không hợp lệ.
3. Chạy cả Schoolbook và Karatsuba với các dữ liệu hợp lệ.
4. Ghi kết quả, độ dài kết quả và thời gian thực thi vào `output.txt`.

Việc chạy test `test100000digits.txt` có thể mất nhiều thời gian vì chương trình thực hiện phép nhân với số có 100.000 chữ số.

### Chạy với hai số cụ thể

Mã nguồn có nhánh xử lý khi chương trình được gọi với hai tham số dòng lệnh (`argc == 3`). Tuy nhiên, phiên bản hiện tại vẫn gọi hàm đọc dữ liệu từ `stdin` trong nhánh này. Vì vậy, để chạy ổn định theo thiết kế hiện tại, hãy chạy chế độ kiểm thử mặc định ở trên hoặc cung cấp hai dòng số qua đầu vào chuẩn:

Windows:
```cmd
BigIntegerArithmetic.exe 123 456
```
Linux:
```terminal
./BigIntegerArithmetic 123 456
```

Kết quả của hai thuật toán trong chế độ này được in trực tiếp ra màn hình. Hai số cần nằm trên hai dòng riêng biệt và chỉ được chứa chữ số, có thể có một dấu `-` ở đầu.

*P/S: Các số trong hai lệnh chạy trên chỉ là ví dụ.

## Định dạng tệp test

Mỗi tệp test hợp lệ phải có đúng hai dòng không rỗng:

```text
123456789
-987654321
```

Các trường hợp được kiểm thử trong thư mục `test\` gồm:

- Số có độ dài 3, 100, 10.000 và 100.000 chữ số.
- Số âm và các tổ hợp dấu.
- Nhân với số 0 và số chỉ có một chữ số.
- Số có các chữ số trùng nhau.
- Số có các chữ số 0 ở đầu.
- Dòng rỗng, ký tự không hợp lệ và dấu âm không hợp lệ.

## Định dạng kết quả

Mỗi lần benchmark ghi các thông tin sau:

```text
Function name: multiply
Num1: ...
Num2: ...
Result: ...
Result length: ...
Execution time: ... seconds
```

Kết quả của Schoolbook và Karatsuba có thể đối chiếu với nhau để kiểm tra tính đúng đắn. Thời gian thực thi phụ thuộc vào máy, trình biên dịch và kích thước dữ liệu.

## Thêm test mới

1. Tạo một tệp `.txt` trong thư mục `test\`.
2. Ghi hai số cần nhân vào hai dòng đầu tiên.
3. Thêm tên tệp vào mảng `testFiles` trong `BigIntegerArithmetic.cpp`.
4. Biên dịch lại chương trình.
5. Chạy `BigIntegerArithmetic.exe` và xem kết quả trong `output.txt`.

## Lưu ý

- Phải chạy chương trình từ thư mục gốc của dự án để đường dẫn tương đối `test\...` hoạt động.
- Mỗi lần chạy chế độ test, `output.txt` được mở ở chế độ ghi mới nên kết quả cũ sẽ bị ghi đè.
- Nếu chương trình không tìm thấy tệp test, hãy kiểm tra thư mục hiện tại và bảo đảm thư mục `test\` tồn tại.
