#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>

template <size_t dimension, typename Type>
class Point {
    public:
        std::array<Type, dimension> values;

        Point() {}
        Point(Type x, Type y) : values { x, y } {}
        Point(Type x, Type y, Type z) : values { x, y, z } {}

        Type& x() { 
            assert(dimension >= 1);
            return values[0]; 
        }
        Type x() const { 
            assert(dimension >= 1);
            return values[0]; 
        }

        Type& y() { 
            assert(dimension >= 2);
            return values[1]; 
        }
        Type y() const { 
            assert(dimension >= 2);
            return values[1]; 
        }

        Type& z() { 
            assert(dimension >= 3);
            return values[2]; 
        }
        Type z() const { 
            assert(dimension >= 3);
            return values[2]; 
        }

        template <typename Op>
        void apply_operation(const Point& other) {
            Op op;
            std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), [op] (Type x1, Type x2) { return op(x1, x2); });
        }

        Point& operator+=(const Point& other) {
            apply_operation<std::plus<Type>>(other);
            return *this;
        }

        Point& operator-=(const Point& other) {
            apply_operation<std::minus<Type>>(other);
            return *this;
        }

        Point& operator*=(const Point& other) {
            apply_operation<std::multiplies<Type>>(other);
            return *this;
        }

        Point& operator*=(const Type scalar){
            std::transform(values.begin(), values.end(), values.begin(), [scalar] (Type x) { return x *= scalar; });
            return *this;
        }

        Point operator*(const Type scalar) const {
            Point result = *this;
            result *= scalar;
            return result;
        }

        Point operator*(const Point& other) const {
            Point result = *this;
            result *= other;
            return result;
        }

        Point operator-(const Point& other) const {
            Point result = *this;
            result -= other;
            return result;
        }

        Point operator+(const Point& other) const {
            Point result = *this;
            result += other;
            return result;
        }

        Point operator-() const {
            // à adapter pour n coordonnées
            return Point { -x(), -y(), -z() }; 
        }

        float length() const {
            return std::sqrt(std::reduce(values.begin(), values.end(), 0., [](Type x1, Type x2) { return x1 + x2 * x2; }));
        }

        float distance_to(const Point& other) const { 
            return (*this - other).length(); 
        }

        Point& normalize(const float target_len = 1.0f) {
            const float current_len = length();
            if (current_len == 0) {
                throw std::logic_error("cannot normalize vector of length 0");
            }

            *this *= (target_len / current_len);
            return *this;
        }

        Point& cap_length(const float max_len) {
            assert(max_len > 0);

            const float current_len = length();
            if (current_len > max_len) {
                *this *= (max_len / current_len);
            }

            return *this;
        }

};

using Point2D = Point<2, float>;
using Point3D = Point<3, float>;

// void test_generic_points() {
//     int a = 5;
//     //Point<2, int> p1;
//     //Point<2, int> p2;
//     //auto p3 = p1 + p2;
//     //p1 += p2;
//     //p1 *= 3; // ou 3.f, ou 3.0 en fonction du type de Point
// }


// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}
