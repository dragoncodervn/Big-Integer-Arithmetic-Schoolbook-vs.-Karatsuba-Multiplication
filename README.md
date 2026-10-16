# Big Integer Arithmetic: Schoolbook vs. Karatsuba Multiplication
## Project Overview
This is the final project for the Data Structures and Algorithms (DSA) course at HCMUTE.
The project implements and compares two big integer multiplication algorithms:
- **Schoolbook:** O(n²) time complexity.
- **Karatsuba:** O(n^1.585) time complexity.
Both algorithms operate on integers represented as strings. The program measures their execution times and saves the results to `output.txt`.

## What Did We Do?

### Week 1 – Basic Implementation

- `add()`: Adds two non-negative integer strings.
- `subtract()`: Subtracts integer strings, supported by `subtractPositive()` and `compareNumbers()`.
- `multiply()`: Performs Schoolbook multiplication.
- `karatsuba()`: Performs multiplication using divide and conquer.
- `benchmark()`: Measures execution time and records results.

### Week 2 – Negative Input Handling

Extended the program to support negative integers and validate input data.

- `multiplySigned()`: Handles operand signs and determines the sign of the result.
- `multiplySignedSchoolbook()`: Integrates signed multiplication with Schoolbook.
- `multiplySignedKaratsuba()`: Integrates signed multiplication with Karatsuba.
- `normalizeNumber()`: Validates input, removes leading zeros, and converts `-000` to `0`.
- `readInput()`: Updated to reject invalid input.
- `main()`: Updated to benchmark both signed multiplication functions.
- **Bug fix:** Corrected the Karatsuba result combination by replacing `p3` with `p2`.

## How to Compile & Run

### Compile

Windows:
```cmd
g++ BigIntegerArithmetic.cpp -o BigIntegerArithmetic.exe
```

Linux:
```bash
g++ BigIntegerArithmetic.cpp -o BigIntegerArithmetic
```

### Run

Windows (PowerShell):
```powershell
.\BigIntegerArithmetic.exe
```

Linux:
```bash
./BigIntegerArithmetic
```

Results are saved to `output.txt`.

## Adding Test Cases

1. Create a `.txt` file in the `test/` folder.
2. Enter two integers, one per line.
3. Add the file name to the `testFiles` array in `main()`.
4. Recompile and run the program.

Example:
```text
-123
45
```
Invalid inputs such as `12a3` or `--123` are skipped.
## Week 2 – Testing Results
**Result:** All 8 test cases passed for the implemented Week 2 features.

