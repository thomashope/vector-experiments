#pragma once

#include <cstdio>
#include <cassert>

typedef float f32;

struct vec2
{
	vec2(f32 x, f32 y) : x(x), y(y) {}
	union
	{
		struct { float x, y; };
	};

	bool operator == (const vec2& rhs) const { return x == rhs.x && y == rhs.y; }
	vec2 operator + (const vec2& rhs) const { return vec2(x + rhs.x, y + rhs.y); }
};

#define swizzle_v2_inline_ops(a, b)\
	bool operator == (const vec2& rhs) const { return a == rhs.x && b == rhs.y; }\
	vec2 operator + (const vec2& rhs) const { return vec2(*this) + rhs; }

// NB: returning void here breaks chaining, but IDK if I ever use that?
#define swizzle_v2_inline_assign_ops(a, b)\
	void operator = (const vec2& rhs) { a = rhs.x; b = rhs.y; }\
	void operator += (const vec2& rhs) { a += rhs.x; b += rhs.y; }

// This is the inline version. Use it directly inside the union.
//
// NB: this only allows one way operator overloads
#define swizzle_v3_v2(a, b) \
	struct { f32 x, y, z; \
	operator vec2 () const { return vec2(a, b); } \
	void operator = (const vec2& rhs) { a = rhs.x; b = rhs.y; } \
	swizzle_v2_inline_assign_ops(a,b) \
	swizzle_v2_inline_ops \
	} a##b;

// This macro is for declaring the type outside of the struct. You have to add it to the union separately.
//
// Unlike the inine version above, this lets you define operator overlaods in both directions.	
#define swizzle_v3_v2_t(a, b) \
	struct vec3_##a##b##_t { f32 x, y, z; \
	operator vec2 () const { return vec2(a, b); } \
	swizzle_v2_inline_assign_ops(a,b) \
	swizzle_v2_inline_ops(a,b) \
	}; \
	bool operator == (const vec2& lhs, const vec3_##a##b##_t& rhs) { return lhs == (vec2)rhs; }
	// vec2 operator + (const vec2& lhs, const vec3_##a##b##_t& rhs) { return lhs + (vec2)rhs; }

#define swizzle_v3_v2_member(a,b) vec3_##a##b##_t a##b;

// NB: splats are not assignable, but AFAIK that doesn't make sense anyway?
#define splat_v3_v2(a, b) \
	struct { f32 x, y, z; \
	operator vec2 () { return vec2(a, b); } \
	} a##b;

#define splat_v3_v3(a, b, c) \
	struct { f32 x, y, z; \
	operator vec3 () { return vec3(a, b, c); } \
	} a##b##c;

#include "vec3_swizzle_types.inl"

struct vec3
{
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

	bool operator == (vec3 rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
};

static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
static_assert(sizeof(vec2) == 2 * sizeof(f32), "vec2 must by 2 f32");
static_assert(sizeof(vec3) == 3 * sizeof(f32), "vec3 must by 3 f32");