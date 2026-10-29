# Big Integer Arithmetic: Schoolbook vs. Karatsuba Multiplication

**Course:** DASA230179_Dot1 - HK1 2026-2027

**Instructor:** Huynh Xuan Phung

**Student(s):** [Add the name(s) and student ID(s); maximum two students]

## 1. Project Overview

This project implements decimal big-integer multiplication using two algorithms:

- **Schoolbook multiplication**, which multiplies each pair of input digits.
- **Karatsuba multiplication**, which recursively splits each operand and reduces the number of recursive multiplications.

The program compares their output and execution time over test inputs ranging from small values to operands with up to 1,000,000 digits. It also benchmarks Karatsuba with several base-case cutoff values.

**Reference:** *The Art of Computer Programming*, Volume 2, Chapter 4: arithmetic and multiplication algorithms.

## 2. Scope and Design

### 2.1 Representation and input handling

Operands are stored as decimal `std::string` values, not as an array or list of multi-digit limbs. This makes decimal input and output straightforward, but string allocation and copying affect performance.

Input handling:

- Accepts an optional leading `-` and decimal digits.
- Rejects empty values, a lone minus sign, whitespace, and non-digit characters.
- Removes leading zeroes while preserving zero as `"0"`.
- Normalizes negative zero to `"0"`.
- Applies sign handling outside the unsigned multiplication algorithms.

For file-based tests, the program reads the first two lines of each input file as the operands. A valid test needs two non-empty lines.

### 2.2 Source files

| File | Purpose |
|---|---|
| `BigIntegerArithmetic.cpp` | Program entry point; selects test files and cutoff values, then runs benchmarks |
| `handleInput.cpp`, `handleInput.hpp` | Input validation, normalization, and unsigned number comparison |
| `schoolbookMultiply.cpp`, `schoolbookMultiply.hpp` | Schoolbook multiplication |
| `karatsuba.cpp`, `karatsuba.hpp` | Big-integer addition, subtraction, and Karatsuba multiplication |
| `multiplySigned.cpp`, `multiplySigned.hpp` | Removes and reapplies operand signs around multiplication |
| `benchmark.cpp`, `benchmark.hpp` | Times a multiplication call and writes operands, result, result length, and elapsed time |
| `test\` | Text input files used by the default test run |
| `output.txt` | Generated benchmark output; overwritten by a default test run |

The repository currently has no separate automated test executable or test framework. The main program runs benchmarks and prints their results; it does not automatically compare every result to an independent expected value.

### 2.3 Schoolbook multiplication

For operands of `p` and `q` digits, the implementation stores partial digits in a result vector and uses nested loops to multiply every digit of one operand by every digit of the other.

### 2.4 Karatsuba multiplication

For two operands split at `m` digits:

```text
x = a * 10^m + b
y = c * 10^m + d

P1 = a * c
P2 = b * d
P3 = (a + b) * (c + d)
ad + bc = P3 - P1 - P2
x * y = P1 * 10^(2m) + (P3 - P1 - P2) * 10^m + P2
```

Karatsuba computes three recursive products rather than four. When either operand has at most `cutoff` digits, the implementation switches to Schoolbook. The benchmark currently tests:

```text
2, 4, 8, 16, 24, 32, 48, 64, 96, 128, 192, 256, 512
```

The cutoff must be greater than zero. There is no single production cutoff selected by the code; the listed values are benchmark candidates.

## 3. Time Complexity

Let `p` and `q` be the operand lengths, and assume `p >= q`. Results expressed in terms of `n` assume operands of approximately equal length (`p ~= q ~= n`).

### 3.1 Schoolbook

The nested loops process `p * q` digit pairs. Initializing the result vector and formatting the answer adds `O(p + q)` work:

```text
T(p, q) = Theta(p * q + p + q) = Theta(p * q)
```

For balanced operands, this becomes **Theta(n^2)**. For operands with very different lengths, the multiplication work depends on `p * q`, not necessarily `n^2`.

### 3.2 Karatsuba under the linear-addition assumption

If addition, subtraction, and result assembly each take linear time, the three recursive calls have size about `n/2`, and the work outside recursion is linear:

```text
T(n) = 3T(n/2) + Theta(n)
```

By the Master Theorem:

```text
T(n) = Theta(n^(log2(3))) ~= Theta(n^1.585)
```

This is the theoretical Karatsuba bound when auxiliary arithmetic is linear. A fixed cutoff changes constants and the base-case work, but not the asymptotic exponent under this model.

### 3.3 Complexity caveat for this implementation

In the current `karatsuba.cpp`, `add` and `subtractPositive` insert each output digit at the beginning of a `std::string` using `insert(result.begin(), ...)`. Inserting at the beginning may shift the digits already in the string, so an addition or subtraction of `n` digits can take **Theta(n^2)** time.

With that cost, the recurrence for the current implementation can be:

```text
T(n) = 3T(n/2) + Theta(n^2) = Theta(n^2)
```

Therefore, distinguish the theoretical Karatsuba bound from the complexity of this specific string implementation. Karatsuba may still be faster for measured input sizes, but the current implementation does not guarantee a Theta(n^1.585) asymptotic running time. Building output from right to left in a buffer and reversing once would avoid repeated front insertion.

### 3.4 Comparison

| Algorithm | Recursive digit multiplications | Balanced-input time | Assumption / note |
|---|---:|---:|---|
| Schoolbook | `p * q` digit pairs | `Theta(n^2)` | Direct nested-loop implementation |
| Karatsuba, theoretical | 3 subproblems of size about `n/2` | `Theta(n^1.585)` | Addition and subtraction are `Theta(n)` |
| Karatsuba, current code | 3 subproblems of size about `n/2` | Can be `Theta(n^2)` | Front insertion can make auxiliary arithmetic quadratic |

## 4. Test Data and Benchmark Behavior

### 4.1 Test cases currently configured

`BigIntegerArithmetic.cpp` lists 26 files:

| Category | Files |
|---|---|
| Small and repeated values | `test3digits.txt`, `test100digits.txt`, `testDuplicateValues.txt`, `testDuplicateDigitsValues.txt` |
| Sign handling | `testNegativePositive.txt`, `testPositiveNegative.txt`, `testNegativeNegative.txt`, `testNegativeZero.txt` |
| Input validation / incomplete input | `testEmptyInput.txt`, `testSingleElement.txt`, `testInvalidCharacters.txt`, `testInvalidSign.txt` |
| Leading zeroes | `testLeadingZeros.txt` |
| Operand sizes | `test1000digits.txt`, `test5000digits.txt`, `test10000digits.txt`, `test20000digits.txt`, `test100000digits.txt`, `test200000digits.txt`, `test300000digits.txt`, `test500000digits.txt`, `test750000digits.txt`, `test1000000digits.txt` |
| Special large operands | `test100000Identity.txt`, `test1000000Identity.txt`, `test200000PowerOfTen.txt` |

`testEmptyInput.txt` is empty, and `testSingleElement.txt` contains only one line; both are intentionally rejected by the two-line input reader. Invalid-character and invalid-sign files are also expected to be skipped. The remaining files are intended as valid multiplication inputs.

### 4.2 What a benchmark run does

With no command-line arguments, the program:

1. Opens each configured input under the relative path `test\`.
2. Reads and validates the first two lines.
3. Runs Schoolbook once and Karatsuba once for each of the 13 cutoff values when the input is valid.
4. Writes the operands, result, result length, and measured multiplication time to `output.txt`.
5. Reports invalid or incomplete inputs as skipped.

The timer covers the multiplication call, not the subsequent output formatting. Since the benchmark writes both large operands and the result for every algorithm/cutoff, `output.txt` can become very large. Each default run overwrites the previous output.

The current test path uses a Windows backslash (`test\\`), so the default file-based runner is Windows-oriented. Running from the repository root is required for its relative paths to resolve.

With exactly two command-line arguments, the program treats them as the operands, validates them, and prints one Schoolbook benchmark plus 13 Karatsuba benchmarks to the console:

```powershell
.\BigIntegerArithmetic.exe 123 456
```

The benchmark output shows results and timing but does not itself assert that Schoolbook and Karatsuba results match. Check correctness separately when changing the arithmetic implementation or cutoff.

### 4.3 Interpreting `output.txt`

`output.txt` is generated data, not a maintained specification. It reflects the last default test run and can be stale if the test list or source has changed since that run. Re-run the program before using its contents as evidence for the current source.

The existing benchmark data has been analyzed using the median of per-test normalized times: for each test, divide each cutoff's time by the fastest cutoff's time for that same test, then take the median across tests. In that data, cutoff **96** had the lowest median ratio (about **1.061x** across 16 complete tests and **1.012x** across 7 tests with at least 1,000 digits). This is an empirical result for one set of runs, not a universal optimum.

Summing raw times can produce a different winner because the largest tests dominate the total; in the analyzed data, cutoff 16 had a slightly lower summed time than cutoff 96. Each case was measured once, so system load and timing noise may materially affect either result. Repeated runs and median timings are recommended before selecting a cutoff for a particular machine.

## 5. Build and Run

### Requirements

- A C++ compiler with C++11 support.
- `g++` / MinGW on Windows.
- Run commands from the repository root so the relative `test\` paths work.

### Build with g++

In Windows PowerShell:

```powershell
g++ -O2 -std=c++11 BigIntegerArithmetic.cpp benchmark.cpp handleInput.cpp karatsuba.cpp multiplySigned.cpp schoolbookMultiply.cpp -o BigIntegerArithmetic.exe
```

`-O2` enables compiler optimizations in the executable. It does not change the algorithm, and the actual speedup depends on the compiler and machine. `-O3` can also be measured, but is not guaranteed to be faster than `-O2`.

### Run the configured test suite

```powershell
.\BigIntegerArithmetic.exe
```

This runs all configured files and overwrites `output.txt`. The largest test inputs can make a full run take a long time.

### Run a pair of numbers

```powershell
.\BigIntegerArithmetic.exe 123 456
```

The two values may include a leading minus sign. Results and timings are printed to the terminal.

## 6. Example and Correctness Evidence

A hand-checkable example is:

```text
678 * 345 = 233910
```

The partial products are `678 * 5 = 3390`, `678 * 40 = 27120`, and `678 * 300 = 203400`; their sum is `233910`. A sample run of the current executable produced `233910` for Schoolbook and the tested Karatsuba cutoffs.

Another Karatsuba recombination example is:

```text
1234 * 5678 = 7006652
```

Large input files demonstrate the scale the program can read and process. They do not alone prove that the entire product is correct. The benchmark currently prints results without an independent expected-result assertion; use exact expected values or an independent verification method when making correctness claims.

## 7. Project Requirements Self-Explanation

1. **Big-integer representation and arithmetic:** operands use decimal strings; normalization, comparison, addition, and subtraction support the multiplication algorithms. This is not a limb-array representation.
2. **Schoolbook:** the nested loops process `p * q` digit pairs, giving `Theta(pq)` time and `Theta(n^2)` for balanced operands.
3. **Karatsuba and cutoff:** the algorithm uses three recursive products and falls back to Schoolbook at the cutoff. The theoretical `Theta(n^1.585)` bound assumes linear auxiliary arithmetic; front insertion in the current string routines can make them quadratic.
4. **Benchmark:** the configured suite includes ordinary, invalid, signed, special-value, and large-input cases. A benchmark run measures one Schoolbook call and 13 Karatsuba cutoffs per valid input.
5. **Recurrence and measured behavior:** under linear addition/subtraction, `T(n) = 3T(n/2) + Theta(n)` solves to `Theta(n^1.585)`. Actual timings depend on this implementation, cutoff, compiler flags, machine, and test data.
6. **Test evidence:** small examples can be checked by hand; large cases should be checked against an independent expected result or verifier. Timing alone is not correctness evidence.
7. **The best choice of cutoff:** 96 is the best choice of cutoff. Based on the median of the normalized time ratios across individual tests, a cutoff of 96 performs best: a median of 1.061× across 16 tests and 1.012× across 7 large tests. 


