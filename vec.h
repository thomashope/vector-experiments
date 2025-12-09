#pragma once

#include <cmath>
#include <cstdio>
#include <cassert>

typedef float f32;

//
// vector operator overloads
//

#define vec2_inline_op(op)\
	vec2 operator op (const vec2& rhs) const { return {x op rhs.x, y op rhs.y}; }\
	vec2 operator op (f32 s) const { return {x op s, y op s}; }\
	vec2& operator op##= (const vec2& rhs) { x op##= rhs.x; y op##= rhs.y; return *this; }\
	vec2& operator op##= (f32 s) { x op##= s; y op##= s; return *this; }

#define vec3_inline_op(op)\
	vec3 operator op (const vec3& rhs) const { return {x op rhs.x, y op rhs.y, z op rhs.z}; }\
	vec3 operator op (f32 s) const { return {x op s, y op s, z op s}; }\
	vec3& operator op##= (const vec3& rhs) { x op##= rhs.x; y op##= rhs.y; z op##= rhs.z; return *this; }\
	vec3& operator op##= (f32 s) { x op##= s; y op##= s; z op##= s; return *this; }

#define vec4_inline_op(op)\
	vec4 operator op (const vec4& rhs) const { return {x op rhs.x, y op rhs.y, z op rhs.z, w op rhs.w}; }\
	vec4 operator op (f32 s) const { return {x op s, y op s, z op s, w op s}; }\
	vec4& operator op##= (const vec4& rhs) { x op##= rhs.x; y op##= rhs.y; z op##= rhs.z; w op##= rhs.w; return *this; }\
	vec4& operator op##= (f32 s) { x op##= s; y op##= s; z op##= s; w op##= s; return *this; }

#define vec_inline_ops(num)\
	bool operator != (const vec##num& rhs) const { return !(*this == rhs); }\
	f32& operator [] (int i) { return (&x)[i]; }\
	const f32& operator [] (int i) const { return (&x)[i]; }\
	vec##num##_inline_op(+) vec##num##_inline_op(-) vec##num##_inline_op(*) vec##num##_inline_op(/)

#define vec_free_op(num, op)\
	inline vec##num operator op (f32 s, const vec##num& v) { return v op s; }

#define vec_free_ops(num)\
	vec_free_op(num, +) vec_free_op(num, -) vec_free_op(num, *) vec_free_op(num, /)

//
// vector swizzles
//

#define swizzle_inline_op(num, op)\
	vec##num operator op (const vec##num& rhs) const { return vec##num(*this) op rhs; }\
	vec##num operator op (const f32& rhs) { return vec##num(*this) op rhs; }

#define swizzle_inline_ops(num)\
	bool operator == (const vec##num& rhs) const { return vec##num(*this) == rhs; }\
	swizzle_inline_op(num, +) swizzle_inline_op(num, -) swizzle_inline_op(num, *) swizzle_inline_op(num, /)

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v2_inline_assign_op(a, b, op)\
	void operator op##= (const vec2& rhs) { a op##= rhs.x; b op##= rhs.y; }

#define swizzle_v2_inline_assign_ops(a, b)\
	void operator = (const vec2& rhs) { a = rhs.x; b = rhs.y; }\
	swizzle_v2_inline_assign_op(a, b, +)\
	swizzle_v2_inline_assign_op(a, b, -)\
	swizzle_v2_inline_assign_op(a, b, *)\
	swizzle_v2_inline_assign_op(a, b, /)

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v3_inline_assign_op(a, b, c, op)\
	void operator op##= (const vec3& rhs) { a op##= rhs.x; b op##= rhs.y; c op##= rhs.z; }

#define swizzle_v3_inline_assign_ops(a, b, c)\
	void operator = (const vec3& rhs) { a = rhs.x; b = rhs.y; c = rhs.z; }\
	swizzle_v3_inline_assign_op(a, b, c, +)\
	swizzle_v3_inline_assign_op(a, b, c, -)\
	swizzle_v3_inline_assign_op(a, b, c, *)\
	swizzle_v3_inline_assign_op(a, b, c, /)

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v4_inline_assign_op(a, b, c, d, op)\
	void operator op##= (const vec4& rhs) { a op##= rhs.x; b op##= rhs.y; c op##= rhs.z; d op##= rhs.w; }

#define swizzle_v4_inline_assign_ops(a, b, c, d)\
	void operator = (const vec4& rhs) { a = rhs.x; b = rhs.y; c = rhs.z; d = rhs.w; }\
	swizzle_v4_inline_assign_op(a, b, c, d, +)\
	swizzle_v4_inline_assign_op(a, b, c, d, -)\
	swizzle_v4_inline_assign_op(a, b, c, d, *)\
	swizzle_v4_inline_assign_op(a, b, c, d, /)

#define swizzle_to_v2(a, b)\
	operator vec2 () const { return vec2(a, b); }\
	swizzle_v2_inline_assign_ops(a, b)\
	swizzle_inline_ops(2)\

#define swizzle_to_v3(a, b, c)\
	operator vec3 () const { return vec3(a, b, c); }\
	swizzle_v3_inline_assign_ops(a, b, c)\
	swizzle_inline_ops(3)\

#define swizzle_to_v4(a, b, c, d)\
	operator vec4 () const { return vec4(a, b, c, d); }\
	swizzle_v4_inline_assign_ops(a, b, c, d)\
	swizzle_inline_ops(4)\

#define swizzle_v2_v2(a, b) struct { f32 x, y; swizzle_to_v2(a, b) } a##b;
#define swizzle_v3_v2(a, b) struct { f32 x, y, z; swizzle_to_v2(a, b) } a##b;
#define swizzle_v3_v3(a, b, c) struct { f32 x, y, z; swizzle_to_v3(a, b, c)	} a##b##c;
#define swizzle_v4_v2(a, b) struct { f32 x, y, z, w; swizzle_to_v2(a, b) } a##b;
#define swizzle_v4_v3(a, b, c) struct { f32 x, y, z, w; swizzle_to_v3(a, b, c) } a##b##c;
#define swizzle_v4_v4(a, b, c, d) struct { f32 x, y, z, w; swizzle_to_v4(a, b, c, d) } a##b##c##d;

//
// vector splats
//

#define splat_inline_op(num, op)\
	vec##num operator op (const vec##num& rhs) const { return vec##num(*this) op rhs; }

#define splat_inline_ops(num)\
	bool operator == (const vec##num& rhs) const { return vec##num(*this) == rhs; }\
	bool operator != (const vec##num& rhs) const { return vec##num(*this) != rhs; }\
	vec##num operator - () const { return -vec##num(*this); }\
	splat_inline_op(num, +) splat_inline_op(num, -) splat_inline_op(num, *) splat_inline_op(num, /)

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

struct vec2
{
	vec2() = default;
	vec2(f32 all) : x(all), y(all) {}
	vec2(f32 x, f32 y) : x(x), y(y) {}
	union
	{
		struct { f32 x, y; };
		#include "vec2_swizzles.inl"
	};

    f32 lengthSquared() const { return x * x + y * y; }
    f32 length() const { return std::sqrt(lengthSquared()); }
    vec2 normalized() const { return *this / length(); }
    vec2 trimmed(f32 length) const { return lengthSquared() > length * length ? normalized() * length : *this; }
    vec2 withLength(f32 length) const { return normalized() * length; }

	bool operator == (const vec2& rhs) const { return x == rhs.x && y == rhs.y; }
	vec2 operator - () const { return {-x, -y}; }
	vec_inline_ops(2)
};

struct vec3
{
	vec3() = default;
	vec3(f32 all) : x(all), y(all), z(all) {}
	vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

	union
	{
		struct { f32 x, y, z; };
		#include "vec3_swizzles.inl"
	};

    f32 lengthSquared() const { return x * x + y * y + z * z; }
    f32 length() const { return std::sqrt(lengthSquared()); }
    vec3 normalized() const { return *this / length(); }
    vec3 trimmed(f32 length) const { return lengthSquared() > length * length ? normalized() * length : *this; }
    vec3 withLength(f32 length) const { return normalized() * length; }

	bool operator == (const vec3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	vec3 operator - () const { return {-x, -y, -z}; }
	vec_inline_ops(3)
};

struct vec4
{
	vec4() = default;
	vec4(f32 all) : x(all), y(all), z(all), w(all) {}
	vec4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}

	union
	{
		struct { f32 x, y, z, w; };
		#include "vec4_swizzles.inl"
	};

    f32 lengthSquared() const { return x * x + y * y + z * z + w * w; }
    f32 length() const { return std::sqrt(lengthSquared()); }
    vec4 normalized() const { return *this / length(); }
    vec4 trimmed(f32 length) const { return lengthSquared() > length * length ? normalized() * length : *this; }
    vec4 withLength(f32 length) const { return normalized() * length; }

	bool operator == (const vec4& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
	vec4 operator - () const { return {-x, -y, -z, -w}; }
	vec_inline_ops(4)
};

vec_free_ops(2)
vec_free_ops(3)
vec_free_ops(4)

//
//
//

inline f32 dot(const vec2& a, const vec2& b) { return a.x * b.x + a.y * b.y; }

inline vec2 abs(const vec2& v) { return { std::abs(v.x), std::abs(v.y) }; }
inline vec3 abs(const vec3& v) { return { std::abs(v.x), std::abs(v.y), std::abs(v.z) }; }
inline vec4 abs(const vec4& v) { return { std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w) }; }

static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
static_assert(sizeof(vec2) == 2 * sizeof(f32), "vec2 must by 2 f32");
static_assert(sizeof(vec3) == 3 * sizeof(f32), "vec3 must by 3 f32");
static_assert(sizeof(vec4) == 4 * sizeof(f32), "vec4 must by 4 f32");