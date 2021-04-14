#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <utility>

template <size_t dimension, typename Type>
class Point {
    private:
        template <typename Op>
        void apply_operation(const Point& other) {
            Op op;
            std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), [op] (Type x1, Type x2) { return op(x1, x2); });
        }

    public:
        std::array<Type, dimension> values;

        Point() {}

        template<typename... Args>
        Point(Type t, Args&&... args) : values { t, static_cast<Type>(std::forward<Args>(args))... } 
        {
            // On vérifie le nombre d'argument passé au constructeur (+1 car le premier argument ne compte pas).
            static_assert(sizeof...(args)+1 == dimension, "You must enter the exact number of coordinates.");
        }


        Type& x() { 
            static_assert(dimension >= 1, "To access this value, the point must be at least one dimension.");
            return values[0]; 
        }
        Type x() const { 
            static_assert(dimension >= 1, "To access this value, the point must be at least one dimension.");
            return values[0]; 
        }

        Type& y() { 
            static_assert(dimension >= 2, "To access this value, the point must be at least two-dimensional.");
            return values[1]; 
        }
        Type y() const { 
            static_assert(dimension >= 2, "To access this value, the point must be at least two-dimensional.");
            return values[1]; 
        }

        Type& z() { 
            static_assert(dimension >= 3, "To access this value, the point must be at least three-dimensional.");
            return values[2]; 
        }
        Type z() const { 
            static_assert(dimension >= 3, "To access this value, the point must be at least three-dimensional.");
            return values[2]; 
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
            Point result = *this;
            std::transform(result.values.begin(), result.values.end(), result.values.begin(), [] (Type x) { return x = -x; });
            return result; 
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
//     Point<2, int> p1;
//     Point<2, int> p2;
//     auto p3 = p1 + p2;
//     p1 += p2;
//     p1 *= 3; // ou 3.f, ou 3.0 en fonction du type de Point

//     Point3D p{1, 2};
// }


// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}
