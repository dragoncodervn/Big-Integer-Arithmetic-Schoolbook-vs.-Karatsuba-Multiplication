# Big-Integer-Arithmetic-Schoolbook-vs.-Karatsuba-Multiplication

## What did we do?
This is the final project for DSA at HCMUTE
We have create 7 functions for evaluate the benchmark of O(n^2) muliply and karatsuba multiply.
- add() is the funcion for summing 2 values in type string (needed for the karatsuba function).
- subtract() is the function for subtracting 2 values in types string(needed for the karatsuba function).
    + subtractPositive() and compareNumbers() is 2 assistant function for subtract() 
- multiply() is the function for multiply 2 numbers in type string by O(n^2) way.
- karatsuba() is the function for multply 2 numbers in type string by karasuba way.
    + karatsuba is the methods to avoid O(n^2) when multiflying 2 big numbers in computer.
- benchmark() is the function for evaluate the runtime of 2 multifly functions and print the results.

## How to compile & run the program
### Compile
```cmd
g++ BigIntergerArithmetic.cpp -o BigIntergerArithmetic
```

### Run
On Windows - Terminal/Command Prompt: 
```cmd
BigIntergerArithmetic.exe
```

On Linux - Terminal:
```terminal
./BigIntergerArithmetic
```

### Adding test-case for the program
From line 32 to 41, in the string-array named testFiles, add the name of the .txt (the test-case file) you want into the array.
Then, recompile the program and run as normal. 