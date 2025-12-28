#ifndef OVERLOADINGGEOMETRYVECTOR_VECTOR_H
#define OVERLOADINGGEOMETRYVECTOR_VECTOR_H

#include <iostream>

class Vec {
    float x, y, z;
    void _copy(const Vec& orig);
    static constexpr float e = 0.000001f;
public:
    Vec(float x = 0, float y = 0, float z = 0);
    ~Vec();
    Vec(const Vec& orig);
    Vec& operator=(const Vec& orig);
    float operator~() const;
    Vec operator+(const Vec& b) const;
    Vec operator-(const Vec& b) const;
    Vec operator+() const;
    Vec operator-() const;
    Vec operator*(const float k) const;

    bool operator==(const Vec& b) const; //impl
    bool operator!=(const Vec& b) const;
    bool operator<(const Vec& b) const; //impl
    bool operator<=(const Vec& b) const; 
    bool operator>(const Vec& b) const; //impl
    bool operator>=(const Vec& b) const;

    Vec& operator++();
    Vec operator++(int);
    Vec& operator--();
    Vec operator--(int);
    float operator[](const int idx) const;
    float& operator[](const int idx);

    friend std::ostream& operator<<(std::ostream& o, const Vec& v);
    friend std::istream& operator>>(std::istream& i, Vec& v);
};

#endif //OVERLOADINGGEOMETRYVECTOR_VECTOR_H