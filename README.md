# Big Integer Arithmetic: Schoolbook vs. Karatsuba Multiplication

**Course:** DASA230179_Dot1 - HK1 2026-2027

**Instructor:** Huynh Xuan Phung

**Student(s):** Nguyen Le Khanh Duy - 25110011, Nguyen Tran Khanh Linh - 25110036

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

Summary of the results derived in this section for the code in this repository:

| | Schoolbook (`multiply`) | Karatsuba (`karatsuba`) |
|---|---|---|
| Balanced operands (`p ~= q ~= n`) | `Theta(n^2)` | `Theta(n^(log2 3)) ~= Theta(n^1.585)` |
| Unbalanced operands (`p >= q`) | `Theta(p * q)` | `Theta(p * q^0.585)` when `q > cutoff` |
| Effect of doubling `n` | time x 4 | time x 3 |
| Space | `Theta(p + q)` | `Theta(n)` |

### 3.1 Conventions and cost model

- `p` and `q` are the digit counts of the two operands with `p >= q`; `n` is the digit count when both operands have about the same length (the *balanced* case); `lambda = log2(3) ~= 1.585`; `c` is the cutoff.
- **Unit cost:** one operation on one digit or character (read, write, comparison, or one arithmetic step on a digit).
- **String operations:** passing a `std::string` by value costs one copy; `push_back` is amortized `O(1)`; `substr`, `operator+`, `reverse`, `find_first_not_of`, and `compare` are linear in the characters they touch.
- The analysis is for this implementation (one decimal digit per character, base 10), not for an optimized limb-based big-integer library. Memory-allocation and cache effects are hidden inside the constants.
- Unless stated otherwise, "time" means the worst case over all inputs of the given length.

### 3.2 Schoolbook multiplication (`multiply`)

#### Cost of each step

| Step in `multiply(num1, num2)` | Work |
|---|---|
| Arguments passed by value | `Theta(p + q)` (two string copies) |
| Tests `num1 == "0"`, `num2 == "0"` | `O(1)` |
| `vector<int> result(p + q, 0)` | `Theta(p + q)` |
| Double loop over `i` in `[0, p)` and `j` in `[0, q)` | exactly `p * q` iterations of a constant-cost body |
| Building `product` (`p + q` appends of one digit) | `Theta(p + q)` |

One iteration of the inner body does one digit multiplication, two additions, one division and one modulo by 10, and three array accesses. Call that constant cost `kappa`.

#### Total

```text
T(p, q) = kappa * p * q + c1 * (p + q) + c0
```

Because `p + q <= 2 * p * q` for `p, q >= 1`, the double loop dominates:

```text
T(p, q) = Theta(p * q)
```

- The number of single-digit multiplications is **exactly `p * q`**.
- The loop bounds depend only on the lengths, so there is no data-dependent shortcut. Best, average, and worst cases are all `Theta(p * q)`, including inputs full of zeroes. Only an operand that is exactly `"0"` returns early.
- **Balanced case:** `Theta(n^2)`. Doubling `n` multiplies the time by 4; multiplying `n` by 10 multiplies it by 100.
- **Unbalanced case:** linear in each operand, `Theta(p * q)`.
- **Space:** `Theta(p + q)` for the digit vector plus the string copies.

### 3.3 Karatsuba multiplication (`karatsuba`)

#### What one call does

Consider a call after both operands have been normalized and zero-padded to `n` digits. Let `h = floor(n/2)`, so each *high* part has `ceil(n/2)` digits and each *low* part has `h` digits.

| Step in `karatsuba` | Work |
|---|---|
| `normalizeUnsigned` on both operands, zero test, base-case test | `Theta(n)` |
| Zero-pad the shorter operand | `Theta(n)` |
| Four `substr` calls (`high1`, `low1`, `high2`, `low2`) | `Theta(n)` |
| `add(low1, high1)` and `add(low2, high2)` | `Theta(n)` each (argument copies, two normalizations, a loop of at most `ceil(n/2) + 1` iterations, one `reverse`, one normalization) |
| Three recursive calls (`p1`, `p2`, `p3`) | `T(.) + T(.) + T(.)` |
| `subtract(subtract(p3, p2), p1)` | `Theta(n)` per `subtract` (compares, then one digit loop, one `reverse`, one normalization) |
| `p1 + string(2h, '0')`, `mid + string(h, '0')` | `Theta(n)` |
| Two `add` calls to combine the three terms | `Theta(n)` each |

Operand sizes keep every string handled outside the recursion within a constant factor of `n`:

| Quantity | Digits |
|---|---|
| `high`, `low` | `ceil(n/2)`, `floor(n/2)` |
| `low + high` (the sums for `p3`) | at most `ceil(n/2) + 1` |
| `p1`, `p2` | at most `n + 1` |
| `p3`, `mid` | at most `n + 3` |
| shifted `p1`, final result | at most `2n + 3` |

The non-recursive work is therefore a constant number of linear passes over strings of length at most `2n + 3`, with no nested loops:

```text
f(n) = a * n + b      (Theta(n))
```

#### Recurrence

When both operands have more than `c` digits:

```text
T(n) = T(ceil(n/2)) + T(floor(n/2)) + T(ceil(n/2) + 1) + f(n)  <=  3 * T(ceil(n/2) + 1) + a * n
```

When one operand has at most `c` digits the call is `multiply(...)`. For the balanced calls of this recursion both operands then have at most `c + 1` digits, so the base case costs `Theta(c^2)`.

The `+ 1` comes from the carry in the sums. It does not change the exponent: let `U(m) = T(m + 2)`. Then

```text
U(m) = T(m + 2) <= 3 * T(ceil(m/2) + 2) + a * (m + 2) = 3 * U(ceil(m/2)) + a * (m + 2) <= 3 * U(ceil(m/2)) + 3a * m
```

which has the standard form `U(m) = 3U(m/2) + O(m)`, so `U(m) = O(m^lambda)` and `T(n) = U(n - 2) = O(n^lambda)`.

#### Solving the recurrence

**1. Master Theorem.** With `a = 3`, `b = 2`, `f(n) = Theta(n)`: `n^(log2 3) = n^lambda`, and `f(n) = O(n^(lambda - epsilon))` with `epsilon = lambda - 1 ~= 0.585 > 0`. This is Case 1:

```text
T(n) = Theta(n^(log2 3)) ~= Theta(n^1.585)
```

**2. Recursion tree** (clean recurrence `T(n) = 3T(n/2) + a*n`, recursion stops at size about `c`):

| Level `k` | Subproblems | Size each | Work per subproblem | Work on the level |
|---:|---:|---:|---:|---:|
| 0 | 1 | `n` | `a * n` | `a * n` |
| 1 | 3 | `n/2` | `a * n/2` | `(3/2) * a * n` |
| 2 | 9 | `n/4` | `a * n/4` | `(3/2)^2 * a * n` |
| `k` | `3^k` | `n/2^k` | `a * n/2^k` | `(3/2)^k * a * n` |

The level costs form a geometric series with ratio `3/2 > 1`, so the deepest levels dominate. With `K = log2(n/c)` levels above the leaves:

```text
internal levels:  sum_{k<K} a*n*(3/2)^k = 2a*n*((3/2)^K - 1),   where (3/2)^K = (n/c)^(lambda-1)
leaves:           3^K = (n/c)^lambda leaves, each costing kappa * c^2

T(n) ~= n^lambda * [ kappa * c^(2 - lambda) + 2a * c^(1 - lambda) ] - 2a * n
      = n^lambda * [ kappa * c^0.415       + 2a * c^(-0.585)    ] - 2a * n
```

For a fixed cutoff the bracket is a constant, so `T(n) = Theta(n^lambda)`.

**3. Substitution (induction).** For `T(n) = 3T(n/2) + a*n`, claim `T(n) <= A * n^lambda - 2a * n` for a large enough constant `A` (chosen so it holds for the base sizes). Then

```text
T(n) <= 3 * (A * (n/2)^lambda - 2a * (n/2)) + a*n = A * n^lambda - 3a*n + a*n = A * n^lambda - 2a*n
```

because `3 * (n/2)^lambda = n^lambda` (since `2^lambda = 3`). Subtracting the lower-order term `2a*n` is what makes the induction close.

**4. Why three products matter.** Splitting without Karatsuba's trick needs four products, `T(n) = 4T(n/2) + Theta(n)`, whose solution is `Theta(n^(log2 4)) = Theta(n^2)`: the same order as Schoolbook. Replacing 4 by 3 moves the exponent from 2 to `log2 3 ~= 1.585`.

#### Role of the cutoff

The formula above shows that `c` changes the constant, not the exponent:

- `kappa * c^0.415` **grows** with `c`: larger Schoolbook leaves cost more per digit.
- `2a * c^(-0.585)` **shrinks** with `c`: fewer recursion levels means fewer string copies and fewer `add`/`subtract` passes.

The sum of a rising and a falling term is U-shaped. Setting the derivative with respect to `c` to zero gives

```text
c* = (2 * 0.585 / 0.415) * (a / kappa) ~= 2.82 * a / kappa
```

The optimum grows with the cost of the linear string passes (`a`) relative to one digit multiplication (`kappa`). Those constants were not measured separately, so the best cutoff must come from experiment (see section 4.3). Because each level halves the size, balanced leaves have between about `c/2` and `c` digits, not exactly `c`.

The effect of the cutoff on the number of single-digit multiplications is deterministic. The table counts them for balanced random operands with `c = 96` using a copy of `multiply` that adds `p * q` to a counter on every base-case call:

| `n` | Karatsuba digit multiplications | Schoolbook `n^2` | `n^2 / Karatsuba` | Growth vs. `n/2` |
|---:|---:|---:|---:|---:|
| 1,000 | 318,678 | 1,000,000 | 3.14 | - |
| 2,000 | 956,901 | 4,000,000 | 4.18 | 3.003 |
| 8,000 | 8,613,207 | 64,000,000 | 7.43 | 3.001 |
| 32,000 | 77,517,598 | 1,024,000,000 | 13.21 | 3.000 |
| 128,000 | 697,635,090 | 16,384,000,000 | 23.49 | 3.000 |
| 256,000 | 2,092,842,644 | 65,536,000,000 | 31.31 | 3.000 |

The count triples each time `n` doubles, which is the `3T(n/2)` structure, whereas Schoolbook quadruples it. Fewer multiplications do not translate one-to-one into time, because Karatsuba adds string copies and `add`/`subtract` passes.

#### Unbalanced operands (`p > q > c`)

Karatsuba pads the shorter operand with leading zeroes up to `p` digits. While `q <= floor(L/2)` for the current long-operand length `L`, the high half of the shorter operand is all zeroes, so `karatsuba(high1, high2)` returns `"0"` immediately. Only two real subcalls remain, each with the long operand halved and the short operand unchanged:

```text
T(L, q) = 2 * T(L/2, q) + Theta(L)     while L >= 2q
```

After about `log2(p/q)` halvings the calls are balanced; there are about `p/q` of them, each costing `Theta(q^lambda)`:

```text
T(p, q) = Theta( (p/q) * q^lambda ) = Theta( p * q^(lambda - 1) ) = Theta( p * q^0.585 )
```

The linear overhead of the upper levels, `Theta(p * log(p/q))`, is of lower order. If `q <= c`, the first test already calls `multiply` and the cost is `Theta(p * q) <= Theta(c * p)`. Compared with Schoolbook's `Theta(p * q)`, the saving is a factor of about `q^0.415`. This is a derivation, not a formal proof; it is checked against deterministic digit-multiplication counts in section 3.5.

#### Data dependence

Because `karatsuba` normalizes its operands and returns `"0"` immediately, inputs with long runs of zeroes are cheaper than random inputs. For example, if one operand is `10^k`, its low half is zero at every level, so one of the three subcalls returns at once and `T(n) = 2T(n/2) + Theta(n) = Theta(n log n)`. `test200000PowerOfTen.txt` has this shape (a 200,000-digit number times `10^199999`). The worst case for Karatsuba is therefore operands with no zero blocks, such as random digits; Schoolbook has no such shortcut. The `Theta(n^lambda)` bound is a worst-case bound, and such special inputs are expected to run faster than random data of the same length.

#### Space

The recursion depth is about `log2(n/c)`. At depth `k` the live strings (the padded operands, the four halves, `p1`, `p2`, `p3`, and temporaries) have `O(n/2^k)` characters, so the total is a geometric series: `Theta(n)` extra space. Schoolbook needs `Theta(p + q)`.

### 3.4 Side-by-side comparison

| | Schoolbook | Karatsuba |
|---|---|---|
| Single-digit multiplications | exactly `p * q` | about `3^depth * leafsize^2`; `Theta(n^lambda)` for fixed cutoff |
| Recurrence | none (double loop) | `T(n) = 3T(n/2) + Theta(n)` |
| Time, balanced | `Theta(n^2)` | `Theta(n^1.585)` |
| Time, `p >> q > c` | `Theta(p * q)` | `Theta(p * q^0.585)` |
| Doubling `n` | x 4 | x 3 |
| Multiplying `n` by 10 | x 100 | x `10^1.585 ~= 38.5` |
| Dependence on digit values | none | zero runs can short-circuit subcalls |
| Per-step overhead | tiny | string copies, `substr`, `add`, `subtract` in every call |
| Extra space | `Theta(p + q)` | `Theta(n)` |

The ideal advantage `n^2 / n^lambda = n^0.415` is about 17.6 at `n = 1,000`, 45.7 at `n = 10,000`, and 119 at `n = 100,000`. Real speedups are smaller because Karatsuba's constant is larger and the cutoff keeps Schoolbook at the leaves; see the measured speedups in section 3.5.

### 3.5 Empirical verification

**Setup:** balanced random operands (random decimal digits, leading digit nonzero), `c = 96`, `g++ -O2 -std=c++11`, Windows desktop with other applications running. Times are medians of 5 runs (`n <= 8,000`), 3 runs (`n <= 32,000`), or one run (larger `n`). Wherever both algorithms were run, their products were identical. Schoolbook was not run above `n = 64,000` because it takes too long.

| `n` | Schoolbook | Karatsuba | Speedup | Schoolbook time vs. `n/2` | Karatsuba time vs. `n/2` |
|---:|---:|---:|---:|---:|---:|
| 1,000 | 4.62 ms | 2.75 ms | 1.68x | - | - |
| 2,000 | 18.0 ms | 7.02 ms | 2.57x | 3.91 | 2.56 |
| 4,000 | 71.6 ms | 20.6 ms | 3.48x | 3.97 | 2.93 |
| 8,000 | 276 ms | 68.0 ms | 4.06x | 3.86 | 3.31 |
| 16,000 | 1.176 s | 0.185 s | 6.36x | 4.26 | 2.72 |
| 32,000 | 4.975 s | 0.542 s | 9.17x | 4.23 | 2.93 |
| 64,000 | 20.33 s | 1.675 s | 12.13x | 4.09 | 3.09 |
| 128,000 | - | 5.311 s | - | - | 3.17 |
| 256,000 | - | 15.68 s | - | - | 2.95 |

- **Growth factor per doubling** (geometric mean): Schoolbook **4.05** (theory 4), Karatsuba **2.95** (theory 3).
- **Log-log slope** (least-squares fit of `log(time)` against `log(n)`): Schoolbook **2.02** (theory 2); Karatsuba **1.57** over `n = 1,000..256,000` and **1.58** for `n >= 8,000` (theory `log2 3 = 1.585`).
- Individual ratios are noisy (for example 3.31 at `n = 8,000`) because most points are single runs on a shared machine. The trend across eight doublings, not a single value, is the evidence.

**Unbalanced operands (deterministic counts).** Wall-clock timings for unbalanced operands varied by more than 2x between repeated runs on this machine (even Schoolbook, which should be linear in `q`, was not monotonic), so they are not reported. Instead the same counter was used to count single-digit multiplications for `p = 200,000` random digits and `c = 96`:

| `q` | Karatsuba digit multiplications | Schoolbook `p * q` | Growth vs. `q/2` | Count / `(p * q^0.585)` |
|---:|---:|---:|---:|---:|
| 500 | 41,552,214 | 100,000,000 | - | 5.48 |
| 1,000 | 62,565,137 | 200,000,000 | 1.506 | 5.50 |
| 2,000 | 94,923,898 | 400,000,000 | 1.517 | 5.56 |
| 4,000 | 143,256,909 | 800,000,000 | 1.509 | 5.60 |
| 8,000 | 214,168,036 | 1,600,000,000 | 1.495 | 5.58 |
| 16,000 | 320,822,870 | 3,200,000,000 | 1.498 | 5.57 |
| 32,000 | 481,155,295 | 6,400,000,000 | 1.500 | 5.57 |

Doubling `q` multiplies the count by about 1.50 (`2^0.585 = 1.50`), against 2 for Schoolbook, and the last column is essentially constant. This supports the `Theta(p * q^0.585)` derivation for the multiplication work.

**Operands of the form `10^k` (data dependence).** For `10^k x 10^k` with `c = 96` the product was correct and the times were 0.038 s, 0.078 s, 0.186 s, 0.472 s, and 1.014 s for `k = 50,000 .. 800,000` (doubling ratios 2.02, 2.40, 2.53, 2.15; single runs). This is far below the time for random operands (for comparison, random operands of 256,000 digits took about 15.7 s above) and is consistent with the `Theta(n log n)` estimate above.

**Effect of the `add`/`subtract` implementation.** The recurrence above is only valid if every non-recursive step is linear. An earlier version of `add` and `subtractPositive` inserted each digit at the front of the string with `insert(result.begin(), ...)`, which shifts all digits already written and makes each addition or subtraction `Theta(n^2)`; it also removed leading zeroes one `erase` at a time. That gives `T(n) = 3T(n/2) + Theta(n^2) = Theta(n^2)`. The current code appends with `push_back`, calls `reverse` once, and strips leading zeroes in a single pass in `normalizeUnsigned`, restoring `f(n) = Theta(n)`. A direct before/after comparison (random balanced operands, `c = 64`, one run per size):

| `n` | Previous version | Current version | Current time vs. `n/2` |
|---:|---:|---:|---:|
| 10,000 | 0.164 s | 0.107 s | - |
| 20,000 | 0.523 s | 0.277 s | 2.58 |
| 40,000 | 1.939 s | 0.803 s | 2.90 |
| 80,000 | 7.049 s | 2.813 s | 3.50 |
| 160,000 | 26.436 s | 8.068 s | 2.87 |
| 320,000 | 98.949 s | 24.484 s | 3.03 |

The previous version's growth factor drifts toward 3.7-3.75 (the quadratic trend); the current one stays near 3. Both returned products identical to Schoolbook in the correctness checks (random operands of different lengths, all nines, powers of ten, zero, and cutoffs 1 to 128).

### 3.6 Assumptions and limitations

- The unit-cost model ignores memory allocation, cache behavior, and the cost of copying strings by value; these affect constants and the cutoff but not the exponents.
- The constants `a` and `kappa` were not measured, so the formula `c* ~= 2.82 * a / kappa` is not evaluated here. The empirical best cutoff comes from the benchmark in section 4.3.
- Timing data are from a shared desktop, mostly single runs. The deterministic operation counts are the reliable evidence for the unbalanced case.
- The `Theta(p * q^0.585)` result for unbalanced operands is a derivation checked numerically, not a formal proof.

### 3.7 Code notes: keeping the non-recursive work linear

The `Theta(n^1.585)` result depends on every non-recursive step in `karatsuba.cpp` being linear:

- `add` and `subtractPositive` append each result digit with `push_back` (amortized `O(1)`; the buffer is reserved up front), then call `reverse` once and `normalizeUnsigned` once. Each addition or subtraction of `n` digits is therefore `Theta(n)`.
- Leading zeroes are removed by a single pass in `normalizeUnsigned`, not by repeatedly erasing the first character (which would be quadratic when the difference has many leading zeroes).
- Splitting (`substr`), padding, appending `10^k` zeroes, and `compareNumbers` are single linear passes.

Reintroducing front insertion (`result.insert(result.begin(), ...)`) or per-character `erase` in these helpers would silently turn the recurrence back into `T(n) = 3T(n/2) + Theta(n^2) = Theta(n^2)`. In the before/after table of section 3.5, a log-log fit over `n = 10,000..320,000` gives a slope of about 1.57 for the current code and about 1.85 for the previous version.

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

The existing `output.txt` data was produced **before** the `add`/`subtractPositive` fix described in sections 3.5 and 3.7, so Karatsuba timings in it are slower than the current code would produce, especially for large inputs. It was analyzed using the median of per-test normalized times: for each test, divide each cutoff's time by the fastest cutoff's time for that same test, then take the median across tests. In that data, cutoff **96** had the lowest median ratio (about **1.061x** across 16 complete tests and **1.012x** across 7 tests with at least 1,000 digits). This is an empirical result for one set of runs, not a universal optimum, and the best cutoff may shift now that the auxiliary arithmetic is cheaper. Re-run the full benchmark with the current code before quoting cutoff or speedup figures.

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
2. **Schoolbook:** the nested loops execute exactly `p * q` constant-cost iterations, so the time is `Theta(p * q)` in every case and `Theta(n^2)` for balanced operands; doubling `n` multiplies the time by about 4 (measured 4.05, log-log slope 2.02; section 3.2 and 3.5).
3. **Karatsuba and cutoff:** the algorithm uses three recursive products and falls back to Schoolbook at the cutoff. Each call does a constant number of linear passes, giving `T(n) = 3T(n/2) + Theta(n)`, solved by the Master Theorem, a recursion tree, and induction. The cutoff changes only the constant (`n^lambda * [kappa * c^0.415 + 2a * c^(-0.585)]`), which is why an interior optimum exists (section 3.3). The linear bound holds because `add` and `subtractPositive` use `push_back` plus a single `reverse` (section 3.7).
4. **Benchmark:** the configured suite includes ordinary, invalid, signed, special-value, and large-input cases. A benchmark run measures one Schoolbook call and 13 Karatsuba cutoffs per valid input.
5. **Recurrence and measured behavior:** `T(n) = 3T(n/2) + Theta(n)` solves to `Theta(n^1.585)`. Measured growth per doubling of `n` is about 2.95 for Karatsuba (theory 3) and the log-log slope is 1.57-1.58 (theory 1.585); the number of base-case digit multiplications triples exactly with each doubling. Actual times still depend on the cutoff, compiler flags, machine, and test data (section 3.5).
6. **Test evidence:** small examples can be checked by hand; large cases should be checked against an independent expected result or verifier. Timing alone is not correctness evidence.
7. **The best choice of cutoff:** 96 is the best choice of cutoff. Based on the median of the normalized time ratios across individual tests, a cutoff of 96 performs best: a median of 1.061× across 16 tests and 1.012× across 7 large tests. 


