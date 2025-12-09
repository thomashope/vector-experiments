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

#define swizzle_inline_op(num, op)\
    vec<Type, num> operator op (const vec<Type, num>& rhs) const { return vec<Type, num>(*this) op rhs; }\
    vec<Type, num> operator op (const Type& rhs) const { return vec<Type, num>(*this) op rhs; }

#define swizzle_inline_ops(num)\
    bool operator == (const vec<Type, num>& rhs) const { return vec<Type, num>(*this) == rhs; }\
    swizzle_inline_op(num, +) swizzle_inline_op(num, -) swizzle_inline_op(num, *) swizzle_inline_op(num, /)

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v2_inline_assign_op(a, b, op)\
    void operator op##= (const vec<Type, 2>& rhs) { a op##= rhs.x; b op##= rhs.y; }

#define swizzle_v2_inline_assign_ops(a, b)\
    void operator = (const vec<Type, 2>& rhs) { a = rhs.x; b = rhs.y; }\
    swizzle_v2_inline_assign_op(a, b, +)\
    swizzle_v2_inline_assign_op(a, b, -)\
    swizzle_v2_inline_assign_op(a, b, *)\
    swizzle_v2_inline_assign_op(a, b, /)

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v3_inline_assign_op(a, b, c, op)\
    void operator op##= (const vec<Type, 3>& rhs) { a op##= rhs.x; b op##= rhs.y; c op##= rhs.z; }

#define swizzle_v3_inline_assign_ops(a, b, c)\
    void operator = (const vec<Type, 3>& rhs) { a = rhs.x; b = rhs.y; c = rhs.z; }\
    swizzle_v3_inline_assign_op(a, b, c, +)\
    swizzle_v3_inline_assign_op(a, b, c, -)\
    swizzle_v3_inline_assign_op(a, b, c, *)\
    swizzle_v3_inline_assign_op(a, b, c, /)

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v4_inline_assign_op(a, b, c, d, op)\
    void operator op##= (const vec<Type, 4>& rhs) { a op##= rhs.x; b op##= rhs.y; c op##= rhs.z; d op##= rhs.w; }

#define swizzle_v4_inline_assign_ops(a, b, c, d)\
    void operator = (const vec<Type, 4>& rhs) { a = rhs.x; b = rhs.y; c = rhs.z; d = rhs.w; }\
    swizzle_v4_inline_assign_op(a, b, c, d, +)\
    swizzle_v4_inline_assign_op(a, b, c, d, -)\
    swizzle_v4_inline_assign_op(a, b, c, d, *)\
    swizzle_v4_inline_assign_op(a, b, c, d, /)

#define swizzle_to_v2(a, b)\
    operator vec<Type, 2> () const { return {a, b}; }\
    swizzle_v2_inline_assign_ops(a, b)\
    swizzle_inline_ops(2)

#define swizzle_to_v3(a, b, c)\
    operator vec<Type, 3> () const { return {a, b, c}; }\
    swizzle_v3_inline_assign_ops(a, b, c)\
    swizzle_inline_ops(3)

#define swizzle_to_v4(a, b, c, d)\
    operator vec<Type, 4> () const { return {a, b, c, d}; }\
    swizzle_v4_inline_assign_ops(a, b, c, d)\
    swizzle_inline_ops(4)

#define swizzle_v2_v2(a, b) struct { Type x, y; swizzle_to_v2(a, b) } a##b;
#define swizzle_v3_v2(a, b) struct { Type x, y, z; swizzle_to_v2(a, b) } a##b;
#define swizzle_v3_v3(a, b, c) struct { Type x, y, z; swizzle_to_v3(a, b, c) } a##b##c;
#define swizzle_v4_v2(a, b) struct { Type x, y, z, w; swizzle_to_v2(a, b) } a##b;
#define swizzle_v4_v3(a, b, c) struct { Type x, y, z, w; swizzle_to_v3(a, b, c) } a##b##c;
#define swizzle_v4_v4(a, b, c, d) struct { Type x, y, z, w; swizzle_to_v4(a, b, c, d) } a##b##c##d;

//
// vector splats
//

#define splat_inline_op(num, op)
    // vec##num operator op (const vec##num& rhs) const { return vec##num(*this) op rhs; }

#define splat_inline_ops(num)
    // bool operator == (const vec##num& rhs) const { return vec##num(*this) == rhs; }
    // bool operator != (const vec##num& rhs) const { return vec##num(*this) != rhs; }
    // vec##num operator - () const { return -vec##num(*this); }
    // splat_inline_op(num, +) splat_inline_op(num, -) splat_inline_op(num, *) splat_inline_op(num, /)

#define splat_to_v2(a, b)\
    operator vec2 () const { return vec2(a, b); }\
    splat_inline_ops(2)\

#define splat_to_v3(a, b, c)\
    operator vec3 () const { return vec3(a, b, c); }\
    splat_inline_ops(3)\

#define splat_to_v4(a, b, c, d)\
    operator vec4 () const { return vec4(a, b, c, d); }\
    splat_inline_ops(4)\

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

// vec + vec
#define op(o) template<typename Type, int Size> vec<Type, Size> operator o (const vec<Type, Size>& a, const vec<Type, Size>& b) { vec<Type, Size> result; for (int i = 0; i < Size; ++i) result[i] = a[i] o b[i]; return result; }
    op(+)
    op(-)
    op(*)
    op(/)
#undef op

// vec + convertible_to_vec (for swizzles on RHS)
// The typename = ... part ensures this doesn't match when Swizzle is vec<Type, Size> (avoiding infinite recursion)
#define op(o) template<typename Type, int Size, typename Swizzle, \
    typename = typename std::enable_if<!std::is_same<Swizzle, vec<Type, Size>>::value>::type> \
auto operator o (const vec<Type, Size>& a, const Swizzle& b) -> decltype(a o vec<Type, Size>(b)) { \
    return a o vec<Type, Size>(b); \
}
    op(+)
    op(-)
    op(*)
    op(/)
#undef op

// vec += vec
#define op(o) template<typename Type, int Size> vec<Type, Size>& operator o (vec<Type, Size>& a, const vec<Type, Size>& b) { for (int i = 0; i < Size; ++i) a[i] o b[i]; return a; }
    op(+=)
    op(-=)
    op(*=)
    op(/=)
#undef op

// vec + scalar
#define op(o) template<typename Type, int Size> vec<Type, Size> operator o (const vec<Type, Size>& a, const f32 scalar) { vec<Type, Size> result; for (int i = 0; i < Size; ++i) result[i] = a[i] o scalar; return result; }
    op(+)
    op(-)
    op(*)
    op(/)
#undef op

// scalar + vec
#define op(o) template<typename Type, int Size> vec<Type, Size> operator o (const f32 scalar, const vec<Type, Size>& a) { return a o scalar; }
    op(+)
    op(-)
    op(*)
    op(/)
#undef op

// vec += scalar
#define op(o) template<typename Type, int Size> vec<Type, Size>& operator o (vec<Type, Size>& a, f32 scalar) { for (int i = 0; i < Size; ++i) a[i] o scalar; return a; }
    op(+=)
    op(-=)
    op(*=)
    op(/=)
#undef op

/// Returns an angle between 0 and PI in radians. Both a and b must be normalized.
///
/// When rays are parallell and pointing in the same direction, the angle is 0. When the they are pointing in opposite directions, the angle is -PI.
f32 angleBetween(const vec2& a, const vec2& b);

/// Returns the angle in radians between the two vectors. Both `a` and `b` should be normalized.
f32 angleBetween(const vec3& a, const vec3& b);

inline f32 dot(const vec2& a, const vec2& b) { return a.x * b.x + a.y * b.y; }
inline f32 dot(const vec3& a, const vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline f32 dot(const vec4& a, const vec4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

/// In 2D `cross` returns the signed area of the parallelogram created by the two vectors.
inline f32 cross(const vec2& a, const vec2& b) { return a.x * b.y - a.y * b.x; }
inline vec3 cross(const vec3& a, const vec3& b) { return { (a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x) }; }

inline vec2 abs(const vec2& v) { return { std::abs(v.x), std::abs(v.y) }; }
inline vec3 abs(const vec3& v) { return { std::abs(v.x), std::abs(v.y), std::abs(v.z) }; }
inline vec4 abs(const vec4& v) { return { std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w) }; }

inline bool isnan(const vec2& v) { return std::isnan(v.x) || std::isnan(v.y); }
inline bool isnan(const vec3& v) { return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z); }
inline bool isnan(const vec4& v) { return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isnan(v.w); }
