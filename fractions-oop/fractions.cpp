#include <bits/stdc++.h>

class Frac {
    int n;   // numerator
    int d;  // denominator

    // Compute GCD
    static int gcd(int a, int b) {
        while (b != 0) {
            int t = a % b;
            a = b;
            b = t;
        }
        return (a < 0 ? -a : a);
    }

    // Reduce the fraction
    void reduce() {
        int g = gcd(n, d);
        n /= g;
        d /= g;

        if (d < 0) {   // keep denominator positive
            d = -d;
            n = -n;
        }
    }

public:
    // Constructors
    Frac(int num = 0, int den = 1) : n(num), d(den) {
        reduce();
    }

    // Accessors
    int N() const { return n; }
    int D() const { return d; }

    // +
    Frac operator+(const Frac &b) const {
        return Frac(n * b.d + b.n * d, d * b.d);
    }
    // -
    Frac operator-(const Frac &b) const {
        return Frac(n * b.d - b.n * d, d * b.d);
    }
    // *
    Frac operator*(const Frac &b) const {
        return Frac(n * b.n, d * b.d);
    }
    // /
    Frac operator/(const Frac &b) const {
        return Frac(n * b.d, d * b.n);
    }

    //==
    bool operator==(const Frac &b) const {
        return n == b.n && d == b.d;
    }
    //<
    bool operator<(const Frac &b) const {
        return (long long)n * b.d < (long long)b.n * d;
    }
    //>
    bool operator>(const Frac &b) const {
        return (long long)n * b.d > (long long)b.n * d;
    }
    // Print function
    void print() const {
        std::cout << n << "/" << d;
    }
};


int main() {
    Frac a(2, 3);
    Frac b(4, 5);

    std::cout << "a = "; a.print(); std::cout << std::endl;
    std::cout << "b = "; b.print(); std::cout << std::endl;

    std::cout << "a + b = "; (a + b).print(); std::cout << std::endl;
    std::cout << "a - b = "; (a - b).print(); std::cout << std::endl;
    std::cout << "a * b = "; (a * b).print(); std::cout << std::endl;
    std::cout << "a / b = "; (a / b).print(); std::cout << std::endl;

    std::cout << "a == b ? " << (a == b ? "yes" : "no") << std::endl;
    std::cout << "a <  b ? " << (a < b ? "yes" : "no") << std::endl;
    std::cout << "a >  b ? " << (a > b ? "yes" : "no") << std::endl;

    return 0;
}
