#pragma once

#include <cstdio>
#include <cassert>

typedef float f32;

struct vec2
{
	vec2() = default;
	vec2(const vec2&) = default;
	vec2(f32 x, f32 y) : x(x), y(y) {}
	union
	{
		struct { float x, y; };
	};

	bool operator == (const vec2& rhs) const { return x == rhs.x && y == rhs.y; }
	vec2 operator + (const vec2& rhs) const { return vec2(x + rhs.x, y + rhs.y); }
};

#define swizzle_inline_ops(num)\
	bool operator == (const vec##num& rhs) const { return vec##num(*this) == rhs; }\
	vec##num operator + (const vec##num& rhs) const { return vec##num(*this) + rhs; }

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v2_inline_assign_ops(a, b)\
	void operator = (const vec2& rhs) { a = rhs.x; b = rhs.y; }\
	void operator += (const vec2& rhs) { a += rhs.x; b += rhs.y; }

// This macro is for declaring the type outside of the struct. You have to add it to the union separately.
#define swizzle_v3_v2_type(a, b) \
	struct vec3_##a##b##_t { f32 x, y, z; \
	operator vec2 () const { return vec2(a, b); } \
	swizzle_v2_inline_assign_ops(a,b) \
	swizzle_inline_ops(2) \
	};

#define swizzle_v3_v2_member(a,b) vec3_##a##b##_t a##b;

#define splat_inline_ops(num)\
	bool operator == (const vec##num& rhs) const { return vec##num(*this) == rhs; }\
	vec##num operator + (const vec##num& rhs) const { return vec##num(*this) + rhs; }

// NB: splats are not assignable, but AFAIK that doesn't make sense anyway?
#define splat_v3_v2(a, b) \
	struct { f32 x, y, z; \
	operator vec2 () const { return vec2(a, b); } \
	splat_inline_ops(2)\
	} a##b;

#define splat_v3_v3(a, b, c) \
	struct { f32 x, y, z; \
	operator vec3 () const { return vec3(a, b, c); } \
	splat_inline_ops(3)\
	} a##b##c;

#include "vec3_swizzle_types.inl"

struct vec3
{
	vec3() = default;
	vec3(const vec3&) = default;
	vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

	union
	{
		struct { f32 x, y, z; };
		#include "vec3_swizzles.inl"

		splat_v3_v2(x, x)
		splat_v3_v2(y, y)
		splat_v3_v2(z, z)
		splat_v3_v3(x, x, y)
	};

	bool operator == (const vec3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	vec3 operator + (const vec3& rhs) const { return vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
};

f32 dot(const vec2& a, const vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
static_assert(sizeof(vec2) == 2 * sizeof(f32), "vec2 must by 2 f32");
static_assert(sizeof(vec3) == 3 * sizeof(f32), "vec3 must by 3 f32");