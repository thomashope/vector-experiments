#pragma once

#include <cmath>
#include <cstring>
#include <type_traits>

typedef float f32;

#define common_operators\
	Type* data() { return &x; }\
	const Type* data() const { return &x; }\
    Type& operator [] (int i) { return (&x)[i]; }\
    const Type& operator [] (int i) const { return (&x)[i]; }

//
// vector swizzles
//

#define swizzle_to_v2(a, b)\
    operator vec<Type, 2> () const { return {a, b}; }\
    void operator = (const vec<Type, 2>& rhs) { a = rhs.x; b = rhs.y; }\
    void operator += (const vec<Type, 2>& rhs) { a += rhs.x; b += rhs.y; }\
    void operator -= (const vec<Type, 2>& rhs) { a -= rhs.x; b -= rhs.y; }\
    void operator *= (const vec<Type, 2>& rhs) { a *= rhs.x; b *= rhs.y; }\
    void operator /= (const vec<Type, 2>& rhs) { a /= rhs.x; b /= rhs.y; }

#define swizzle_to_v3(a, b, c)\
    operator vec<Type, 3> () const { return {a, b, c}; }\
    void operator = (const vec<Type, 3>& rhs) { a = rhs.x; b = rhs.y; c = rhs.z; }\
    void operator += (const vec<Type, 3>& rhs) { a += rhs.x; b += rhs.y; c += rhs.z; }\
    void operator -= (const vec<Type, 3>& rhs) { a -= rhs.x; b -= rhs.y; c -= rhs.z; }\
    void operator *= (const vec<Type, 3>& rhs) { a *= rhs.x; b *= rhs.y; c *= rhs.z; }\
    void operator /= (const vec<Type, 3>& rhs) { a /= rhs.x; b /= rhs.y; c /= rhs.z; }

#define swizzle_to_v4(a, b, c, d)\
    operator vec<Type, 4> () const { return {a, b, c, d}; }\
    void operator = (const vec<Type, 4>& rhs) { a = rhs.x; b = rhs.y; c = rhs.z; d = rhs.w; }\
    void operator += (const vec<Type, 4>& rhs) { a += rhs.x; b += rhs.y; c += rhs.z; d += rhs.w; }\
    void operator -= (const vec<Type, 4>& rhs) { a -= rhs.x; b -= rhs.y; c -= rhs.z; d -= rhs.w; }\
    void operator *= (const vec<Type, 4>& rhs) { a *= rhs.x; b *= rhs.y; c *= rhs.z; d *= rhs.w; }\
    void operator /= (const vec<Type, 4>& rhs) { a /= rhs.x; b /= rhs.y; c /= rhs.z; d /= rhs.w; }

#define swizzle_v2_v2(a, b) struct { Type x, y; swizzle_to_v2(a, b) } a##b;
#define swizzle_v3_v2(a, b) struct { Type x, y, z; swizzle_to_v2(a, b) } a##b;
#define swizzle_v3_v3(a, b, c) struct { Type x, y, z; swizzle_to_v3(a, b, c) } a##b##c;
#define swizzle_v4_v2(a, b) struct { Type x, y, z, w; swizzle_to_v2(a, b) } a##b;
#define swizzle_v4_v3(a, b, c) struct { Type x, y, z, w; swizzle_to_v3(a, b, c) } a##b##c;
#define swizzle_v4_v4(a, b, c, d) struct { Type x, y, z, w; swizzle_to_v4(a, b, c, d) } a##b##c##d;

//
// vector splats
//

#define splat_to_v2(a, b)\
    operator vec2 () const { return vec2(a, b); }

#define splat_to_v3(a, b, c)\
    operator vec3 () const { return vec3(a, b, c); }

#define splat_to_v4(a, b, c, d)\
    operator vec4 () const { return vec4(a, b, c, d); }

// NB: splats are not assignable, but AFAIK that doesn't make sense anyway?
#define splat_v2_v2(a, b) struct { f32 x, y; splat_to_v2(a, b) } a##b;
#define splat_v3_v2(a, b) struct { f32 x, y, z; splat_to_v2(a, b) } a##b;
#define splat_v3_v3(a, b, c) struct { f32 x, y, z; splat_to_v3(a, b, c) } a##b##c;
#define splat_v4_v2(a, b) struct { f32 x, y, z, w; splat_to_v2(a, b) } a##b;
#define splat_v4_v3(a, b, c) struct { f32 x, y, z, w; splat_to_v3(a, b, c) } a##b##c;
#define splat_v4_v4(a, b, c, d) struct { f32 x, y, z, w; splat_to_v4(a, b, c, d) } a##b##c##d;

//
// vector types
//

template<typename Type, int Size>
struct vec
{
    Type v[Size] = {};

    Type lengthSquared() const
    {
        Type result;
        for (int i = 0; i < Size; ++i)
            result += v[i] * v[i];
        return result;
    }

    Type length() const { return std::sqrt(lengthSquared()); }
    vec normalized() const { return *this / length(); }

    bool operator == (const vec& rhs) { return std::memcmp(v, rhs.v, sizeof(v)) == 0; }
};

typedef vec<f32, 2> vec2;
typedef vec<f32, 3> vec3;
typedef vec<f32, 4> vec4;

//
// specialization for vec2
//

template<typename Type>
struct vec<Type, 2>
{
    union
    {
        struct { Type x, y; };
        #include "vec2_swizzles.inl"
    };

    vec() : x((Type)0), y((Type)0) {}
    explicit vec(Type valueForAll) : x(valueForAll), y(valueForAll) {}
    vec(Type x, Type y) : x(x), y(y) {}

    /// 0 points along the positive x axis. PI/2 points along the positive Y axis.
    static vec fromAngle(f32 radians) { return { std::cos(radians), std::sin(radians) }; }

    f32 lengthSquared() const { return x * x + y * y; }
    f32 length() const { return std::sqrt(lengthSquared()); }
    f32 angle() const { return std::atan2(y, x); }; /// Returns the angle in radins to the positive X axis.
    vec rotated(f32 radians) const { f32 s = std::sin(radians), c = std::cos(radians); return { x * c - y * s, x * s + y * c }; }
    vec normalized() const { return *this / length(); }
    vec trimmed(f32 length) const { return lengthSquared() > length * length ? normalized() * length : *this; }
    vec withLength(f32 length) const { return normalized() * length; }
    bool isNan() const { return std::isnan(x) || std::isnan(y); }

    common_operators
    bool operator == (const vec& rhs) const { return x == rhs.x && y == rhs.y; }
    vec operator - () const { return { Type(-x), Type(-y) }; }
};

//
// specialization for vec3
//

template<typename Type>
struct vec<Type, 3>
{
    union
    {
        struct { Type x, y, z; };
        #include "vec3_swizzles.inl"
    };

    vec() : x((Type)0), y((Type)0), z((Type)0) {}
    explicit vec(Type valueForAll) : x(valueForAll), y(valueForAll), z(valueForAll) {}
    explicit vec(const Type* data) : x(data[0]), y(data[1]), z(data[2]) {}
    vec(Type x, Type y, Type z) : x(x), y(y), z(z) {}
    vec(vec<Type, 2> xy, Type z) : x(xy.x), y(xy.y), z(z) {}

    f32 lengthSquared() const { return x * x + y * y + z * z; }
    f32 length() const { return std::sqrt(lengthSquared()); }
    vec normalized() const { f32 l = length(); return l ? *this / l : *this; }
    vec2 pitchYaw() const { vec dir = normalized(); return { std::asin(dir.z), std::atan2(dir.y, dir.x) }; } /// Returns a vec2 where X is the pitch and Y is the yaw.
    vec trimmed(f32 length) const { return lengthSquared() > length * length ? normalized() * length : *this; }
    vec withLength(f32 length) const { return normalized() * length; }
    bool isNan() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }

    common_operators
    bool operator == (const vec& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    vec operator - () const { return { -x, -y, -z }; }
};

template<typename Type>
struct vec<Type, 4>
{
    union
    {
        struct { Type x, y, z, w; };
        #include "vec4_swizzles.inl"
    };

    vec() : x((Type)0), y((Type)0), z((Type)0), w((Type)0) {}
    explicit vec(Type valueForAll) : x(valueForAll), y(valueForAll), z(valueForAll), w(valueForAll) {}
    explicit vec(const Type* data) : x(data[0]), y(data[1]), z(data[2]), w(data[3]) {}
    vec(Type x, Type y, Type z, Type w) : x(x), y(y), z(z), w(w) {}
    vec(vec<Type, 3> xyz, Type w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

    f32 lengthSquared() const { return x * x + y * y + z * z + w * w; }
    f32 length() const { return std::sqrt(lengthSquared()); }
    vec normalized() const { f32 l = length(); return l ? *this / l : *this; }

    common_operators
    bool operator == (const vec& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
    vec operator - () const { return { -x, -y, -z, -w }; }    
};

template<typename Type, int Size>
bool operator != (const vec<Type, Size>& a, const vec<Type, Size>& b)
{
    return !(a == b);
}

//
// C++20 Concepts for type-safe vector operations
//

// Check if type is exactly a vec type
template<typename T> concept IsVec2 = std::same_as<T, vec2>;
template<typename T> concept IsVec3 = std::same_as<T, vec3>;
template<typename T> concept IsVec4 = std::same_as<T, vec4>;
template<typename T> concept IsVec = IsVec2<T> || IsVec3<T> || IsVec4<T>;

// Check if type is convertible to vec (includes swizzles)
template<typename T> concept ConvertibleToVec2 = std::convertible_to<T, vec2>;
template<typename T> concept ConvertibleToVec3 = std::convertible_to<T, vec3>;
template<typename T> concept ConvertibleToVec4 = std::convertible_to<T, vec4>;

// Check if type is a scalar (for vec-scalar operations)
template<typename T> concept Scalar = std::convertible_to<T, f32>;

//
// Binary arithmetic operators: +, -, *, /
// These handle all combinations of vec, swizzle, and scalar
//

// vec + vec (for exact vec types)
template<typename Type, int Size>
vec<Type, Size> operator + (const vec<Type, Size>& a, const vec<Type, Size>& b) {
    vec<Type, Size> result;
    for (int i = 0; i < Size; ++i) result[i] = a[i] + b[i];
    return result;
}

template<typename Type, int Size>
vec<Type, Size> operator - (const vec<Type, Size>& a, const vec<Type, Size>& b) {
    vec<Type, Size> result;
    for (int i = 0; i < Size; ++i) result[i] = a[i] - b[i];
    return result;
}

template<typename Type, int Size>
vec<Type, Size> operator * (const vec<Type, Size>& a, const vec<Type, Size>& b) {
    vec<Type, Size> result;
    for (int i = 0; i < Size; ++i) result[i] = a[i] * b[i];
    return result;
}

template<typename Type, int Size>
vec<Type, Size> operator / (const vec<Type, Size>& a, const vec<Type, Size>& b) {
    vec<Type, Size> result;
    for (int i = 0; i < Size; ++i) result[i] = a[i] / b[i];
    return result;
}

// vec + convertible (e.g., vec2 + swizzle) and convertible + vec (e.g., swizzle + vec2)
#define VEC_OP(op) \
template<ConvertibleToVec2 T1, ConvertibleToVec2 T2> requires (!IsVec2<T1> || !IsVec2<T2>) \
vec2 operator op (const T1& a, const T2& b) { return vec2(a) op vec2(b); } \
template<ConvertibleToVec3 T1, ConvertibleToVec3 T2> requires (!IsVec3<T1> || !IsVec3<T2>) \
vec3 operator op (const T1& a, const T2& b) { return vec3(a) op vec3(b); } \
template<ConvertibleToVec4 T1, ConvertibleToVec4 T2> requires (!IsVec4<T1> || !IsVec4<T2>) \
vec4 operator op (const T1& a, const T2& b) { return vec4(a) op vec4(b); }

VEC_OP(+)
VEC_OP(-)
VEC_OP(*)
VEC_OP(/)
#undef VEC_OP

// vec + scalar (proper implementation)
template<typename Type, int Size, Scalar S>
vec<Type, Size> operator + (const vec<Type, Size>& v, const S& s) {
    vec<Type, Size> result;
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) result[i] = v[i] + scalar;
    return result;
}

template<typename Type, int Size, Scalar S>
vec<Type, Size> operator - (const vec<Type, Size>& v, const S& s) {
    vec<Type, Size> result;
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) result[i] = v[i] - scalar;
    return result;
}

template<typename Type, int Size, Scalar S>
vec<Type, Size> operator * (const vec<Type, Size>& v, const S& s) {
    vec<Type, Size> result;
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) result[i] = v[i] * scalar;
    return result;
}

template<typename Type, int Size, Scalar S>
vec<Type, Size> operator / (const vec<Type, Size>& v, const S& s) {
    vec<Type, Size> result;
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) result[i] = v[i] / scalar;
    return result;
}

// scalar + vec (symmetric operations)
template<typename Type, int Size, Scalar S>
vec<Type, Size> operator + (const S& s, const vec<Type, Size>& v) {
    return v + s;
}

template<typename Type, int Size, Scalar S>
vec<Type, Size> operator - (const S& s, const vec<Type, Size>& v) {
    vec<Type, Size> result;
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) result[i] = scalar - v[i];
    return result;
}

template<typename Type, int Size, Scalar S>
vec<Type, Size> operator * (const S& s, const vec<Type, Size>& v) {
    return v * s;
}

template<typename Type, int Size, Scalar S>
vec<Type, Size> operator / (const S& s, const vec<Type, Size>& v) {
    vec<Type, Size> result;
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) result[i] = scalar / v[i];
    return result;
}

//
// Compound assignment operators: +=, -=, *=, /=
//

// vec += vec (for exact vec types)
template<typename Type, int Size>
vec<Type, Size>& operator += (vec<Type, Size>& a, const vec<Type, Size>& b) {
    for (int i = 0; i < Size; ++i) a[i] += b[i];
    return a;
}

template<typename Type, int Size>
vec<Type, Size>& operator -= (vec<Type, Size>& a, const vec<Type, Size>& b) {
    for (int i = 0; i < Size; ++i) a[i] -= b[i];
    return a;
}

template<typename Type, int Size>
vec<Type, Size>& operator *= (vec<Type, Size>& a, const vec<Type, Size>& b) {
    for (int i = 0; i < Size; ++i) a[i] *= b[i];
    return a;
}

template<typename Type, int Size>
vec<Type, Size>& operator /= (vec<Type, Size>& a, const vec<Type, Size>& b) {
    for (int i = 0; i < Size; ++i) a[i] /= b[i];
    return a;
}

// vec += scalar (proper implementation without recursion)
template<typename Type, int Size, Scalar S>
vec<Type, Size>& operator += (vec<Type, Size>& a, const S& s) {
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) a[i] += scalar;
    return a;
}

template<typename Type, int Size, Scalar S>
vec<Type, Size>& operator -= (vec<Type, Size>& a, const S& s) {
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) a[i] -= scalar;
    return a;
}

template<typename Type, int Size, Scalar S>
vec<Type, Size>& operator *= (vec<Type, Size>& a, const S& s) {
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) a[i] *= scalar;
    return a;
}

template<typename Type, int Size, Scalar S>
vec<Type, Size>& operator /= (vec<Type, Size>& a, const S& s) {
    f32 scalar = f32(s);
    for (int i = 0; i < Size; ++i) a[i] /= scalar;
    return a;
}

//
// Comparison operators: ==, !=
//

// vec == convertible and convertible == vec (handles swizzle comparisons)
template<ConvertibleToVec2 T1, ConvertibleToVec2 T2> requires (!IsVec2<T1> || !IsVec2<T2>)
bool operator == (const T1& a, const T2& b) { return vec2(a) == vec2(b); }

template<ConvertibleToVec3 T1, ConvertibleToVec3 T2> requires (!IsVec3<T1> || !IsVec3<T2>)
bool operator == (const T1& a, const T2& b) { return vec3(a) == vec3(b); }

template<ConvertibleToVec4 T1, ConvertibleToVec4 T2> requires (!IsVec4<T1> || !IsVec4<T2>)
bool operator == (const T1& a, const T2& b) { return vec4(a) == vec4(b); }

template<ConvertibleToVec2 T1, ConvertibleToVec2 T2>
bool operator != (const T1& a, const T2& b) { return !(a == b); }

template<ConvertibleToVec3 T1, ConvertibleToVec3 T2>
bool operator != (const T1& a, const T2& b) { return !(a == b); }

template<ConvertibleToVec4 T1, ConvertibleToVec4 T2>
bool operator != (const T1& a, const T2& b) { return !(a == b); }

//
// Unary operators
//

template<ConvertibleToVec2 T> requires (!IsVec2<T>)
vec2 operator - (const T& v) { return -vec2(v); }

template<ConvertibleToVec3 T> requires (!IsVec3<T>)
vec3 operator - (const T& v) { return -vec3(v); }

template<ConvertibleToVec4 T> requires (!IsVec4<T>)
vec4 operator - (const T& v) { return -vec4(v); }

//
// Additional operators for types convertible to vec (using SFINAE/decltype for swizzles)
// These handle swizzle + scalar and scalar + swizzle where implicit conversion doesn't work through template deduction
//

#define SWIZZLE_SCALAR_OPS(VecType) \
template<typename T, typename S> \
requires (!std::same_as<T, VecType> && !std::same_as<S, VecType> && Scalar<S>) \
auto operator + (const T& v, const S& s) -> decltype(VecType(v) + VecType()) { return VecType(v) + f32(s); } \
template<typename T, typename S> \
requires (!std::same_as<T, VecType> && !std::same_as<S, VecType> && Scalar<S>) \
auto operator - (const T& v, const S& s) -> decltype(VecType(v) + VecType()) { return VecType(v) - f32(s); } \
template<typename T, typename S> \
requires (!std::same_as<T, VecType> && !std::same_as<S, VecType> && Scalar<S>) \
auto operator * (const T& v, const S& s) -> decltype(VecType(v) + VecType()) { return VecType(v) * f32(s); } \
template<typename T, typename S> \
requires (!std::same_as<T, VecType> && !std::same_as<S, VecType> && Scalar<S>) \
auto operator / (const T& v, const S& s) -> decltype(VecType(v) + VecType()) { return VecType(v) / f32(s); } \
template<typename S, typename T> \
requires (!std::same_as<T, VecType> && !std::same_as<S, VecType> && Scalar<S>) \
auto operator + (const S& s, const T& v) -> decltype(VecType(v) + VecType()) { return f32(s) + VecType(v); } \
template<typename S, typename T> \
requires (!std::same_as<T, VecType> && !std::same_as<S, VecType> && Scalar<S>) \
auto operator - (const S& s, const T& v) -> decltype(VecType(v) + VecType()) { return f32(s) - VecType(v); } \
template<typename S, typename T> \
requires (!std::same_as<T, VecType> && !std::same_as<S, VecType> && Scalar<S>) \
auto operator * (const S& s, const T& v) -> decltype(VecType(v) + VecType()) { return f32(s) * VecType(v); } \
template<typename S, typename T> \
requires (!std::same_as<T, VecType> && !std::same_as<S, VecType> && Scalar<S>) \
auto operator / (const S& s, const T& v) -> decltype(VecType(v) + VecType()) { return f32(s) / VecType(v); }

SWIZZLE_SCALAR_OPS(vec2)
SWIZZLE_SCALAR_OPS(vec3)
SWIZZLE_SCALAR_OPS(vec4)
#undef SWIZZLE_SCALAR_OPS

/// Returns an angle between 0 and PI in radians. Both a and b must be normalized.
///
/// When rays are parallell and pointing in the same direction, the angle is 0. When the they are pointing in opposite directions, the angle is -PI.
f32 angleBetween(const vec2& a, const vec2& b);

/// Returns the angle in radians between the two vectors. Both `a` and `b` should be normalized.
f32 angleBetween(const vec3& a, const vec3& b);

inline f32 dot(const vec2& a, const vec2& b) { return a.x * b.x + a.y * b.y; }
inline f32 dot(const vec3& a, const vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline f32 dot(const vec4& a, const vec4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

// dot() overloads for swizzles
template<ConvertibleToVec2 T1, ConvertibleToVec2 T2>
f32 dot(const T1& a, const T2& b) { return dot(vec2(a), vec2(b)); }

template<ConvertibleToVec3 T1, ConvertibleToVec3 T2>
f32 dot(const T1& a, const T2& b) { return dot(vec3(a), vec3(b)); }

template<ConvertibleToVec4 T1, ConvertibleToVec4 T2>
f32 dot(const T1& a, const T2& b) { return dot(vec4(a), vec4(b)); }

/// In 2D `cross` returns the signed area of the parallelogram created by the two vectors.
inline f32 cross(const vec2& a, const vec2& b) { return a.x * b.y - a.y * b.x; }
inline vec3 cross(const vec3& a, const vec3& b) { return { (a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x) }; }

inline vec2 abs(const vec2& v) { return { std::abs(v.x), std::abs(v.y) }; }
inline vec3 abs(const vec3& v) { return { std::abs(v.x), std::abs(v.y), std::abs(v.z) }; }
inline vec4 abs(const vec4& v) { return { std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w) }; }

inline bool isnan(const vec2& v) { return std::isnan(v.x) || std::isnan(v.y); }
inline bool isnan(const vec3& v) { return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z); }
inline bool isnan(const vec4& v) { return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isnan(v.w); }
