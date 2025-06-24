#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "vec.h"

void print(vec3 v) { printf("%.3f, %.3f, %.3f\n", v.x, v.y, v.z); }
void print(vec2 v) { printf("%.3f, %.3f\n", v.x, v.y); }

void print_ref(const vec2& v)
{
	print(v);
}

void modify_ref(vec2& v)
{
	v.x = 1337;
	v.y = 42;
}

TEST_CASE( "Vector swizzles" )
{
	vec3 a(1, 2, 3);
	vec2 b(0, 0);
	print(a);
	print(b);

	CHECK(a == a);
	CHECK(a == vec3(1, 2, 3));
	CHECK(b == b);
	CHECK(b == vec2(0, 0));

	b = a.xz; // assign swizzle to vec2

	CHECK((vec2)a.xz == b); // FIXME: requires cast when on LHS
	CHECK(b == a.xz);

	const vec2 c(4, 5);
	a = vec3(1, 2, 3);
	a.xz = c;
	CHECK(c == a.xz);
	CHECK(a == vec3(4, 2, 5));

	a.yz = vec2(6, 7);
	CHECK(a == vec3(4, 6, 7));

	a.xz = vec2(8, 9);
	CHECK(a == vec3(8, 6, 9));

	a.xz = vec2(1, 1);
	CHECK(a == vec3(1, 6, 1));

	a.xz = vec2(a.xz) + vec2(a.xz);
	CHECK(a == vec3(2, 6, 2));
	a.xz = vec2(a.xz) + a.xz;
	CHECK(a == vec3(4, 6, 4));
	a.xz = a.xz + vec2(a.xz);
	CHECK(a == vec3(8, 6, 8));
	a.xz = a.xz + a.xz;
	CHECK(a == vec3(16, 6, 16));

	//modify_ref(a.xy); // FIXME? compile error, better than silent failure...

	a = vec3(1,1,1);
	print_ref(a.xy);
	a.xz += vec2(1,1);
	CHECK(a == vec3(2,1,2));

	a.z = 1;
	a.xy += a.zz;
	CHECK(a == vec3(3,2,1));
}