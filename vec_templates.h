#pragma once

#include <cmath>
#include <cstring>
#include <type_traits>

typedef float f32;

// VEC + VEC - done
// VEC + SWIZZLE - done
// SWIZZLE + VEC - done
// SWIZZLE + SWIZZLE - done

// VEC + SCALAR
// SCALAR + VEC
// SWIZZLE + SCALAR
// SCALAR + SWIZZLE

#define vec2_inline_op(op)\
    vec<Type,2> operator op (const vec<Type,2>& rhs) const { return {x op rhs.x, y op rhs.y}; }\
    vec<Type,2> operator op (Type s) const { return {x op s, y op s}; }\
    vec<Type,2>& operator op##= (const vec<Type,2>& rhs) { x op##= rhs.x; y op##= rhs.y; return *this; }\
    vec<Type,2>& operator op##= (Type s) { x op##= s; y op##= s; return *this; }

#define vec3_inline_op(op)\
    vec<Type,3> operator op (const vec<Type,3>& rhs) const { return {x op rhs.x, y op rhs.y, z op rhs.z}; }\
    vec<Type,3> operator op (Type s) const { return {x op s, y op s, z op s}; }\
    vec<Type,3>& operator op##= (const vec<Type,3>& rhs) { x op##= rhs.x; y op##= rhs.y; z op##= rhs.z; return *this; }\
    vec<Type,3>& operator op##= (Type s) { x op##= s; y op##= s; z op##= s; return *this; }

#define vec4_inline_op(op)\
    vec<Type,4> operator op (const vec<Type,4>& rhs) const { return {x op rhs.x, y op rhs.y, z op rhs.z, w op rhs.w}; }\
    vec<Type,4> operator op (Type s) const { return {x op s, y op s, z op s, w op s}; }\
    vec<Type,4>& operator op##= (const vec<Type,4>& rhs) { x op##= rhs.x; y op##= rhs.y; z op##= rhs.z; w op##= rhs.w; return *this; }\
    vec<Type,4>& operator op##= (Type s) { x op##= s; y op##= s; z op##= s; w op##= s; return *this; }

// Convert VEC + SWIZZLE -> VEC + VEC
#define vec_to_swizzle_inline_op(op, num)\
    template<VecOrSwizzle<Type, num> VecType>\
    vec<Type,num> operator op (const VecType& rhs) const { return *this op (vec<Type,num>)rhs; }\

#define vec_inline_ops(num)\
    bool operator != (const vec##num& rhs) const { return !(*this == rhs); }\
    Type* data() { return &x; }\
    const Type* data() const { return &x; }\
    f32& operator [] (int i) { return (&x)[i]; }\
    const f32& operator [] (int i) const { return (&x)[i]; }\
    vec_to_swizzle_inline_op(+, num) vec_to_swizzle_inline_op(-, num) vec_to_swizzle_inline_op(*, num) vec_to_swizzle_inline_op(/, num)\
    vec##num##_inline_op(+) vec##num##_inline_op(-) vec##num##_inline_op(*) vec##num##_inline_op(/)

//
// vector swizzles
//

#define swizzle_inline_op(num, op)\
    /* Convert SWIZZLE + SWIZZLE -> VEC + VEC */\
    template<VecOrSwizzle<Type, num> RhsType>\
    vec<Type,num> operator op (const RhsType& rhs) const { return vec<Type,num>(*this) op vec<Type,num>(rhs); }\
    /* Accept SWIZZLE + SCALAR, for any SCALAR convertible to Type */\
    template<ConvertibleToScalar<Type> RhsType>\
    vec<Type,num> operator op (const RhsType& rhs) const { return vec<Type,num>(*this) op rhs; }

#define swizzle_inline_ops(num)\
    template<VecOrSwizzle<Type, num> RhsType>\
    bool operator == (const RhsType& rhs) const { return vec<Type,num>(*this) == rhs; }\
    swizzle_inline_op(num, +) swizzle_inline_op(num, -) swizzle_inline_op(num, *) swizzle_inline_op(num, /)

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v2_inline_assign_op(a, b, op)\
    void operator op (const vec<Type,2>& rhs) { a op rhs.x; b op rhs.y; }

#define swizzle_v2_inline_assign_ops(a, b)\
    swizzle_v2_inline_assign_op(a, b, =)\
    swizzle_v2_inline_assign_op(a, b, +=)\
    swizzle_v2_inline_assign_op(a, b, -=)\
    swizzle_v2_inline_assign_op(a, b, *=)\
    swizzle_v2_inline_assign_op(a, b, /=)

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v3_inline_assign_op(a, b, c, op)\
    void operator op (const vec<Type,3>& rhs) { a op rhs.x; b op rhs.y; c op rhs.z; }

#define swizzle_v3_inline_assign_ops(a, b, c)\
    swizzle_v3_inline_assign_op(a, b, c, =)\
    swizzle_v3_inline_assign_op(a, b, c, +=)\
    swizzle_v3_inline_assign_op(a, b, c, -=)\
    swizzle_v3_inline_assign_op(a, b, c, *=)\
    swizzle_v3_inline_assign_op(a, b, c, /=)

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v4_inline_assign_op(a, b, c, d, op)\
    void operator op (const vec<Type,4>& rhs) { a op rhs.x; b op rhs.y; c op rhs.z; d op rhs.w; }

#define swizzle_v4_inline_assign_ops(a, b, c, d)\
    swizzle_v4_inline_assign_op(a, b, c, d, =)\
    swizzle_v4_inline_assign_op(a, b, c, d, +=)\
    swizzle_v4_inline_assign_op(a, b, c, d, -=)\
    swizzle_v4_inline_assign_op(a, b, c, d, *=)\
    swizzle_v4_inline_assign_op(a, b, c, d, /=)

#define swizzle_to_v2(a, b)\
    operator vec<Type, 2> () const { return {a, b}; }\
    swizzle_inline_ops(2)\
    swizzle_v2_inline_assign_ops(a, b)

#define swizzle_to_v3(a, b, c)\
    operator vec<Type, 3> () const { return {a, b, c}; }\
    swizzle_inline_ops(3)\
    swizzle_v3_inline_assign_ops(a, b, c)

#define swizzle_to_v4(a, b, c, d)\
    operator vec<Type, 4> () const { return {a, b, c, d}; }\
    swizzle_inline_ops(4)\
    swizzle_v4_inline_assign_ops(a, b, c, d)

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
    operator vec<Type, 2> () const { return { a, b }; }\
    vec<Type,2> operator - () { return -vec<Type,2>(*this); }\
    swizzle_inline_ops(2)

#define splat_to_v3(a, b, c)\
    operator vec<Type, 3> () const { return { a, b, c }; }\
    vec<Type,3> operator - () { return -vec<Type,3>(*this); }\
    swizzle_inline_ops(3)

#define splat_to_v4(a, b, c, d)\
    operator vec<Type, 4> () const { return { a, b, c, d }; }\
    vec<Type,4> operator - () { return -vec<Type,4>(*this); }\
    swizzle_inline_ops(4)

// NB: splats are not assignable, but AFAIK that doesn't make sense anyway?
#define splat_v2_v2(a, b) struct { Type x, y; splat_to_v2(a, b) } a##b;
#define splat_v3_v2(a, b) struct { Type x, y, z; splat_to_v2(a, b) } a##b;
#define splat_v3_v3(a, b, c) struct { Type x, y, z; splat_to_v3(a, b, c) } a##b##c;
#define splat_v4_v2(a, b) struct { Type x, y, z, w; splat_to_v2(a, b) } a##b;
#define splat_v4_v3(a, b, c) struct { Type x, y, z, w; splat_to_v3(a, b, c) } a##b##c;
#define splat_v4_v4(a, b, c, d) struct { Type x, y, z, w; splat_to_v4(a, b, c, d) } a##b##c##d;

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

template<typename T, typename Type, int Size> concept VecOrSwizzle = std::convertible_to<T, vec<Type,Size>>;
template<typename T, typename ScalarType> concept ConvertibleToScalar = std::convertible_to<T, ScalarType>;

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

    vec_inline_ops(2)

    bool operator == (const vec& rhs) const { return x == rhs.x && y == rhs.y; }
    vec operator - () const { return { -x, -y }; }
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

    vec_inline_ops(3)

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

    vec_inline_ops(4)

    bool operator == (const vec& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
    vec operator - () const { return { -x, -y, -z, -w }; }
};

template<typename Type, int Size>
bool operator != (const vec<Type, Size>& a, const vec<Type, Size>& b)
{
    return !(a == b);
}

#define SCALAR_VEC_OP(op, type, num)\
template<ConvertibleToScalar<type> LhsType, VecOrSwizzle<type, num> RhsType>\
vec<type, num> operator op (const LhsType& scalar, const RhsType& vec) { return ::vec<type, num>(scalar) op vec; }

SCALAR_VEC_OP(+, f32, 2)
SCALAR_VEC_OP(-, f32, 2)
SCALAR_VEC_OP(*, f32, 2)
SCALAR_VEC_OP(/, f32, 2)
SCALAR_VEC_OP(+, f32, 3)
SCALAR_VEC_OP(-, f32, 3)
SCALAR_VEC_OP(*, f32, 3)
SCALAR_VEC_OP(/, f32, 3)
SCALAR_VEC_OP(+, f32, 4)
SCALAR_VEC_OP(-, f32, 4)
SCALAR_VEC_OP(*, f32, 4)
SCALAR_VEC_OP(/, f32, 4)

#undef SCALAR_VEC_OP

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
