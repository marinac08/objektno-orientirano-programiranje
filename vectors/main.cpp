#include <iostream>
#include "vector.cpp"

int main() {
    Vec v(3, 7, 15);
    Vec v0;                        // default constructor
    Vec v2 = v;                     // copy constructor
    std::cout << "v: " << v << "\n";
    std::cout << "v0: " << v0 << "\n";
    std::cout << "v2 (copy of v): " << v2 << "\n";

    // Test increment/decrement
    v++;
    std::cout << "v++: " << v << "\n";
    ++v;
    std::cout << "++v: " << v << "\n";
    v--;
    std::cout << "v--: " << v << "\n";
    --v;
    std::cout << "--v: " << v << "\n";

    // Test scalar multiplication
    v = v * 3;
    std::cout << "v*3: " << v << "\n";
    v2 = v * 0.7f;
    std::cout << "v2=v*0.7: " << v2 << "\n";

    // Test vector addition/subtraction
    std::cout << "v+v2: " << v+v2 << "\n";
    std::cout << "v-v2: " << v-v2 << "\n";
    std::cout << "v2-v2: " << v2-v2 << "\n";

    // Test magnitude
    std::cout << "~v: " << ~v << ", ~v2: " << ~v2 << "\n";

    // Test comparisons
    std::cout << "(v==v2): " << (v==v2) << ", (v!=v2): " << (v!=v2) << "\n";
    std::cout << "(v>v2): " << (v>v2) << ", (v>=v2): " << (v>=v2) << "\n";
    std::cout << "(v<v2): " << (v<v2) << ", (v<=v2): " << (v<=v2) << "\n";

    // Test assignment
    v2 = v;
    std::cout << "\nAfter v2=v assignment:\n";
    std::cout << "(v==v2): " << (v==v2) << ", (v!=v2): " << (v!=v2) << "\n";
    std::cout << "(v>v2): " << (v>v2) << ", (v>=v2): " << (v>=v2) << "\n";
    std::cout << "(v<v2): " << (v<v2) << ", (v<=v2): " << (v<=v2) << "\n";

    // Test operator[]
    std::cout << "\nv[0]=" << v[0] << ", v[1]=" << v[1] << ", v[2]=" << v[2] << "\n";
    v[0] = 42; v[1] = 43; v[2] = 44;
    std::cout << "After modifying v[]: " << v << "\n";

    // Test input operator
    /*
    Vec vin;
    std::cout << "Enter vector values (x y z): ";
    std::cin >> vin;
    std::cout << "You entered: " << vin << "\n";
    */

    // Test epsilon comparison
    v2 = v;
    v2 = v2 + 0.000001f;  // small change
    std::cout << "(v==v2) after tiny change: " << (v==v2) << "\n";
    std::cout << "\n\n";
    std::cout << v << "\n";
    std::cout << ++v << "\n";
    std::cout << (v++) << "\n";

    return 0;
}