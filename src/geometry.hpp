#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>

template <int dim, typename T> struct Point
{
    std::array<std::enable_if_t<std::is_arithmetic_v<T>, T>, dim> values;

public:
    Point() = default;

    template <typename... Args>
    explicit Point(Args&&... args) : values { std::forward<T>(static_cast<T>(args))... }
    {
        static_assert(sizeof...(args) == dim, "dimension must match number of arguments");
    }

    Point& operator+=(const Point& rhs)
    {
        std::transform(values.begin(), values.end(), rhs.values.begin(), values.begin(), std::plus<T>());
        return *this;
    }

    Point& operator-=(const Point& rhs)
    {
        std::transform(values.begin(), values.end(), rhs.values.begin(), values.begin(), std::minus<T>());
        return *this;
    }

    Point& operator*=(const float scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(), [scalar](T& x) { return x * scalar; });
        return *this;
    }

    Point& operator*=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       std::multiplies<T>());
        return *this;
    }

    Point<dim, T> operator+(const Point<dim, T>& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator*(const float scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    Point operator*(const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result;
    }

    T length() const
    {
        return std::sqrt(std::inner_product(values.begin(), values.end(), values.begin(), T {}));
    }

    T distance_to(const Point& other) const { return (*this - other).length(); }

    Point cap_length(const float max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }

    Point normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    T x() const
    {
        static_assert(dim >= 1, "dimension must be at least 1");
        return values[0];
    }

    T y() const
    {
        static_assert(dim >= 2, "dimension must be at least 2");
        return values[1];
    }

    T z() const
    {
        static_assert(dim >= 3, "dimension must be at least 3");
        return values[2];
    }

    T& x()
    {
        static_assert(dim >= 1, "dimension must be at least 1");
        return values[0];
    }

    T& y()
    {
        static_assert(dim >= 2, "dimension must be at least 2");
        return values[1];
    }

    T& z()
    {
        static_assert(dim >= 3, "dimension must be at least 3");
        return values[2];
    }

    Point operator-() const
    {
        Point result = {};
        std::transform(values.begin(), values.end(), result.values.begin(), std::negate<T>());
        return result;
    }

    const std::string to_string() const { return std::string(values.begin(), values.end()); }
};

using Point2D = Point<2, float>;
using Point3D = Point<3, float>;
// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return Point2D { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}
