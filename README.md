📐 Mathematical Foundation of Shamir's Secret Sharing
🎯 Overview
This document provides a comprehensive mathematical explanation of Shamir's Secret Sharing scheme and the algorithms used in our implementation.
🔢 Core Mathematical Concepts
1. Polynomial Interpolation Theory
Definition
Given k distinct points (x₀, y₀), (x₁, y₁), ..., (xₖ₋₁, yₖ₋₁), there exists a unique polynomial P(x) of degree at most k-1 such that P(xᵢ) = yᵢ for all i.
Uniqueness Theorem
Theorem: If two polynomials of degree less than k agree at k distinct points, then they are identical.
Proof Sketch: The difference of two such polynomials would be a non-zero polynomial of degree less than k with k roots, which is impossible unless it's the zero polynomial.
2. Lagrange Interpolation Formula
Complete Formula
For points (x₀,y₀), (x₁,y₁), ..., (xₖ₋₁,yₖ₋₁), the interpolating polynomial is:
P(x) = Σᵢ₌₀^(k-1) yᵢ × Lᵢ(x)
where the Lagrange basis polynomials are:
Lᵢ(x) = ∏ⱼ₌₀,ⱼ≠ᵢ^(k-1) (x - xⱼ)/(xᵢ - xⱼ)
Properties of Basis Polynomials

Kronecker Delta Property: Lᵢ(xⱼ) = δᵢⱼ where δᵢⱼ is the Kronecker delta
Partition of Unity: Σᵢ₌₀^(k-1) Lᵢ(x) = 1 for all x

Specialization at x = 0
For secret recovery, we evaluate at x = 0:
P(0) = Σᵢ₌₀^(k-1) yᵢ × Lᵢ(0)
where:
Lᵢ(0) = ∏ⱼ₌₀,ⱼ≠ᵢ^(k-1) (0 - xⱼ)/(xᵢ - xⱼ) = ∏ⱼ₌₀,ⱼ≠ᵢ^(k-1) (-xⱼ)/(xᵢ - xⱼ)
3. Shamir's Secret Sharing Mathematics
Scheme Parameters

n: Total number of shares
k: Threshold (minimum shares needed for reconstruction)
S: Secret value
p: Prime modulus (for finite field arithmetic, not used in our implementation)

Share Generation

Choose a polynomial of degree k-1:
f(x) = a₀ + a₁x + a₂x² + ... + aₖ₋₁x^(k-1)
where a₀ = S (the secret)
Generate shares: (i, f(i)) for i = 1, 2, ..., n

Secret Reconstruction
Given any k shares (x₁,y₁), ..., (xₖ,yₖ):
S = f(0) = Σᵢ₌₁^k yᵢ × ∏ⱼ₌₁,ⱼ≠ᵢ^k (-xⱼ)/(xᵢ - xⱼ)
🧮 Numerical Methods and Precision
1. Arbitrary Precision Arithmetic
Why Needed?
Standard integer types have limited range:

int: ~2.1 × 10⁹
long long: ~9.2 × 10¹⁸
Test cases contain numbers with 40+ digits

BigInteger Implementation
Our implementation uses string-based arithmetic:
cppclass BigInt {
    string digits;  // Stored in reverse order for easier arithmetic
    bool negative;
    
    // Core operations: add, subtract, multiply, divide
};
Complexity Analysis
For numbers with d digits:

Addition/Subtraction: O(d)
Multiplication: O(d²) (naive), O(d log d) (FFT-based)
Division: O(d²)

2. Exact Rational Arithmetic
Fraction Class
To avoid precision loss in division operations:
cppclass Fraction {
    BigInt numerator, denominator;
    
    void reduce() {
        BigInt g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
    }
};
Why Exact Arithmetic?
Floating-point arithmetic introduces errors:
1/3 + 1/3 + 1/3 ≠ 1  (in floating point)
1/3 + 1/3 + 1/3 = 1   (in exact arithmetic)
🔄 Base Conversion Mathematics
General Base Conversion
From Base b to Decimal
For number dₙdₙ₋₁...d₁d₀ in base b:
Value = Σᵢ₌₀^n dᵢ × b^i
Algorithm Implementation
cppBigInt baseToDecimal(string digits, int base) {
    BigInt result = 0;
    BigInt power = 1;
    
    // Process from least significant digit
    for (int i = digits.length() - 1; i >= 0; i--) {
        int digit = charToDigit(digits[i]);
        result += digit * power;
        power *= base;
    }
    return result;
}
Supported Bases

Binary (2): digits {0, 1}
Octal (8): digits {0, 1, 2, 3, 4, 5, 6, 7}
Decimal (10): digits {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
Hexadecimal (16): digits {0-9, A-F}
General (2-36): digits {0-9, A-Z}

Error Analysis
Rounding Errors
In floating-point arithmetic, each operation can introduce error:
ε_machine ≈ 2.22 × 10^(-16)  (double precision)
Our exact arithmetic eliminates these errors completely.
Overflow Protection
BigInt prevents integer overflow by using dynamic digit storage.
🎲 Worked Examples
Example 1: Simple Reconstruction
Given Points: (1,4), (2,7), (3,12)
Step 1: Calculate Lagrange basis at x=0
L₀(0) = (0-2)(0-3)/((1-2)(1-3)) = (-2)(-3)/((-1)(-2)) = 6/2 = 3
L₁(0) = (0-1)(0-3)/((2-1)(2-3)) = (-1)(-3)/((1)(-1)) = 3/(-1) = -3  
L₂(0) = (0-1)(0-2)/((3-1)(3-2)) = (-1)(-2)/((2)(1)) = 2/2 = 1
Step 2: Compute secret
S = y₀×L₀(0) + y₁×L₁(0) + y₂×L₂(0)
S = 4×3 + 7×(-3) + 12×1
S = 12 - 21 + 12 = 3
Verification: The polynomial is f(x) = 3 + x + 0x², and indeed:

f(1) = 3 + 1 = 4 ✓
f(2) = 3 + 2 = 5 ✗

Wait, let me recalculate...
Correct Calculation:
For points (1,4), (2,7), (3,12):
The polynomial should be f(x) = ax² + bx + c where:

f(1) = a + b + c = 4
f(2) = 4a + 2b + c = 7
f(3) = 9a + 3b + c = 12

Solving this system:

3a + b = 3 (equation 2 - equation 1)
5a + b = 5 (equation 3 - equation 1)
Therefore: 2a = 2, so a = 1
b = 0, c = 3

So f(x) = x² + 3, and f(0) = 3.
Example 2: Large Number Base Conversion
Convert: "2122212201122002221120200210011020220200" from base 3
Calculation:
Value = 2×3⁰ + 0×3¹ + 0×3² + ... (40 terms)
This requires arbitrary precision arithmetic due to the magnitude involved.
📊 Complexity and Performance Analysis
Time Complexity
OperationComplexityNotesBase ConversionO(d log b)d = digits, b = baseLagrange InterpolationO(k²)k = thresholdBigInt AdditionO(max(d₁,d₂))Linear in digit countBigInt MultiplicationO(d₁×d₂)Quadratic (naive)GCD CalculationO(log min(a,b))For fraction reduction
Space Complexity

Point Storage: O(n) where n is number of points
BigInt Storage: O(d) where d is number of digits
Intermediate Results: O(k×d) for k computations

Optimization Opportunities

Karatsuba Multiplication: O(n^log₂3) ≈ O(n^1.585)
FFT Multiplication: O(n log n) for very large numbers
Precomputed Factorials: For repeated interpolations

🔐 Security Considerations
Information Theoretic Security
Shamir's scheme provides perfect secrecy:

Any k-1 shares reveal no information about the secret
Exactly k shares are needed for reconstruction

Practical Security Issues

Side-Channel Attacks: Timing analysis of arithmetic operations
Memory Leaks: Sensitive data in memory dumps
Implementation Bugs: Integer overflow, precision errors

Our implementation addresses:

✅ Precision Errors: Using exact arithmetic
✅ Integer Overflow: Using arbitrary precision
⚠️ Side-Channels: Not explicitly addressed (out of scope)

This mathematical foundation ensures our implementation is both theoretically sound and practically robust.
