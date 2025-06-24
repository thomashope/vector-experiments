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

	vec2 operator + (const vec2& rhs) { return vec2(x + rhs.x, y + rhs.y); }
};

#define swizzle_v2_ops\
	vec2 operator + (const vec2& rhs) { return vec2(*this) + rhs; } \

#define swizzle_v2(a, b) \
	struct { f32 x, y, z; \
	operator vec2 () { return vec2(a, b); } \
	void operator = (const vec2& rhs) { a = rhs.x; b = rhs.y; } \
	swizzle_v2_ops \
	} a##b;

struct vec3
{
	vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

	union
	{
		struct { f32 x, y, z; };
		swizzle_v2(x, y)
		swizzle_v2(y, z)
		swizzle_v2(x, z)
	};

	bool operator == (vec3 rhs) { return x == rhs.x && y == rhs.y && z == rhs.z; }
};

void print(vec3 v) { printf("%.3f, %.3f, %.3f\n", v.x, v.y, v.z); }
void print(vec2 v) { printf("%.3f, %.3f\n", v.x, v.y); }

static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
static_assert(sizeof(vec2) == 2 * sizeof(f32), "vec2 must by 2 f32");
static_assert(sizeof(vec3) == 3 * sizeof(f32), "vec3 must by 3 f32");

void print_ref(const vec2& v)
{
	print(v);
}

void modify_ref(vec2& v)
{
	v.x = 1337;
	v.y = 42;
}

int main()
{
	vec3 a(1, 2, 3);
	vec2 b(0, 0);

	printf("hello swizzle\n");
	print(a);
	print(b);

	print(a.xz); // convert swizzle to type as argument

	b = a.xz; // assign swizzle to vec2
	print(b);

	printf("assigning to swizzles\n");
	print(a);
	a.xz = vec2(4, 5);
	print(a);
	assert(a == vec3(4, 2, 5));
	a.yz = vec2(6, 7);
	print(a);
	a.xz = vec2(8, 9);
	print(a);
	a.xz = vec2(1, 1);
	print(a);
	a.xz = vec2(a.xz) + vec2(a.xz);
	print(a);
	a.xz = vec2(a.xz) + a.xz;
	print(a);
	a.xz = a.xz + vec2(a.xz);
	print(a);
	a.xz = a.xz + a.xz;
	print(a);
	//modify_ref(a.xy); // compile error, better than silent failure.
	print(a);
	print_ref(a.xy);
}