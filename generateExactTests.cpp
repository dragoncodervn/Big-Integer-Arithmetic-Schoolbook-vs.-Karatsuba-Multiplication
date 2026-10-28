
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Tao so nguyen lon bang cach lap lai cac chu so
string generateLargeNumber(int n) 
{
    string number;
    number.reserve(n);
    for (int i = 0; i < n; i++) 
    {
        number += char('1' + (i % 9));
    }
    return number;
}
// Ghi hai so vao file test
void createTest(const string& filename,const string& num1,const string& num2) 
{
ofstream file("test/" + filename);
    if (!file) 
    {
        cout << "Cannot create " << filename << endl;
        return;
    }
    file << num1 << '\n';
    file << num2 << '\n';
    cout << "Created: " << filename << endl;
}
int main() 
{
    // Test 1: A * 1 = A
    createTest("test100000Identity.txt",generateLargeNumber(100000),"1");

    // Test 2: A * 10^199999
    createTest("test200000PowerOfTen.txt",generateLargeNumber(200000),"1" + string(199999, '0'));

    // Test 3: A * 1 = A voi 1 trieu chu so
    createTest("test1000000Identity.txt",generateLargeNumber(1000000),"1");
    return 0;
}
