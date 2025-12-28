#include "vector.h"
#include <cmath>

void Vec::_copy(const Vec& orig) {
    this->x = orig.x;
    this->y = orig.y;
    this->z = orig.z;
}

Vec::Vec(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
Vec::~Vec() {}
Vec::Vec(const Vec &orig) { _copy(orig); }
Vec& Vec::operator=(const Vec& orig) {
    if (this != &orig) _copy(orig);
    return *this;
}

float Vec::operator~() const { return sqrt(this->x*this->x+this->y*this->y+this->z*this->z); }
Vec Vec::operator*(const float k) const { return Vec(this->x*k, this->y*k, this->z*k); } //scalar
Vec Vec::operator+(const Vec& b) const { return Vec(this->x+b.x, this->y+b.y, this->z+b.z); }
Vec Vec::operator-(const Vec& b) const { return Vec(this->x-b.x, this->y-b.y, this->z-b.z); }
Vec Vec::operator+() const { return *this; }
Vec Vec::operator-() const { return (*this)*-1; }

bool Vec::operator==(const Vec& b) const { return (~(*this)-~b<e && ~(*this)-~b>-e); }
bool Vec::operator!=(const Vec& b) const { return !(*this==b); }
bool Vec::operator<(const Vec& b)  const { return ~(*this)<~b+e; }
bool Vec::operator>(const Vec& b)  const { return ~(*this)>~b-e; }
bool Vec::operator<=(const Vec& b) const { return (*this < b) || (*this == b); }
bool Vec::operator>=(const Vec& b) const { return (*this > b) || (*this == b); }

Vec Vec::operator++(int) { //post
    Vec temp(*this);
    this->x++;
    this->y++;
    this->z++;
    return temp;
}
Vec& Vec::operator++() { //pre
    this->x++;
    this->y++;
    this->z++;
    return *this;
}
Vec Vec::operator--(int) {
    Vec temp(*this);
    this->x--;
    this->y--;
    this->z--;
    return temp;
}
Vec& Vec::operator--() {
    this->x--;
    this->y--;
    this->z--;
    return *this;
}

float Vec::operator[](const int idx) const {
    if (idx%3==0) return this->x;
    if (idx%3==1) return this->y;
    return this->z;
}

float& Vec::operator[](const int idx) {
    if (idx%3==0) return this->x;
    if (idx%3==1) return this->y;
    return this->z;
}

std::ostream& operator<<(std::ostream& o, const Vec& v) {
    o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return o;
}
std::istream& operator>>(std::istream& i, Vec& v) {
    i >> v.x >> v.y >> v.z;
    return i;
}