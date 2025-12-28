#include<iostream>
using namespace std;

// +, -, *, /, ==, <, >
// d, d, d, d, d, d, d


int gcd(int a, int b) {
    while (b != 0) {
        const int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int lcm(int a, int b) {
    return a*b/gcd(a, b);
}

class Frac {
    int n;
    int d;
public:
    int N () const { return this->n; }
    int D () const { return this->d; }
    void setN(int x) { this->n = x; }
    void setD(int x) { this->d = x; }

    Frac(const int n, const int d=1) {
        this->n = n;
        this->d = d;
    }

    // Copy C-tor
    Frac (const Frac& orig) {
        this->n = orig.n;
        this->d = orig.d;
    }

    Frac& operator=(const Frac& orig) {
        if (this!=&orig) { // No Self-assignment
            this->n = orig.n;
            this->d = orig.d;
        }
        return *this;
    }

    void print() {
        cout<<this->n <<"/" << this->d;
    }

    Frac normalize() const {
        Frac f(*this);
        const int g = gcd(f.N(), f.D());
        f.setN(f.N() / g);
        f.setD(f.D() / g);
        return f;
    }

    Frac operator*(const Frac& b) const {
        Frac c(*this);
        c.setN(this->N() * b.N());
        c.setD(this->D() * b.D());
        return c.normalize();
        //return Frac(this->N() * b.N(), this->D() * b.D()).normalize();
        ///all the //return functions avoid making a new Frac c, by instantly returning Frac of the result, but basically the same thing, ig?
    }

    Frac operator/(const Frac& b) const {
        Frac c(*this);
        c.setN(this->N() * b.D());
        c.setD(this->D() * b.N());
        return c.normalize();
        //return Frac(this->N() * b.D(), this->D() * b.N()).normalize();
    }

    Frac operator+(const Frac& b) const {
        Frac c(*this);
        const int i=lcm(c.D(), b.D());
        c.setN(c.N()*i/c.D()+b.N()*i/b.D());
        c.setD(i);
        return c.normalize();
        //return Frac(this->N() * b.D() + b.N() * this->D(),this->D() * b.D()).normalize();
    }

    Frac operator-(const Frac& b) const {
        Frac c(*this);
        const int i=lcm(c.D(), b.D());
        c.setN(c.N()*i/c.D()-b.N()*i/b.D());
        c.setD(i);
        return c.normalize();
        //return Frac(this->N() * b.D() - b.N() * this->D(),this->D() * b.D()).normalize();
    }

     bool operator==(const Frac& b) const {
        const Frac c(this->normalize());
        const Frac d = b.normalize();
        return c.N()==d.N() && c.D() == d.D();
    }

    bool operator<(const Frac& b) const {
        return this->N() * b.D() < b.N() * this->D();
    }

    bool operator>(const Frac& b) const {
        return this->N() * b.D() > b.N() * this->D();
    }

    bool operator<=(const Frac& b) const {
        return this->N() * b.D() <= b.N() * this->D();
    }

    bool operator>=(const Frac& b) const {
        return this->N() * b.D() >= b.N() * this->D();
    }

    /*
    bool operator<(const Frac& b) const { return this->N() * b.D() < b.N() * this->D(); }
    bool operator>(const Frac& b) const { return b < *this; }
    bool operator<=(const Frac& b) const { return !(*this > b); } ///!> = <=
    bool operator>=(const Frac& b) const { return !(*this < b); }
    */


    ~Frac() {}

};

ostream& operator<<(ostream& o, const Frac& f) {
    o<<f.N() <<"/" << f.D();
    return o;
}


// Frac operator*(const Frac& a, const Frac& b) {
//     Frac c(a);
//     c.setN(a.N() * b.N());
//     c.setD(a.D() * b.D());
//     return c;
// }

int main() {
    Frac a(2,3);
    Frac b(4,5);
    Frac c(1);
    c.operator=(a.operator*(b));

    // Global function operator* (Frac, Frac)
    // Member Function operator* (Frac)
    // a * b
    // operator*(a,b)
    // a.operator*(b)

    //.c.print();
    unsigned int x = 10;

    cout<<x;

}