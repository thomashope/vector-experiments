#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#ifdef VEC_TEMPLATES
#include "vec_templates.h"
#else
#include "vec.h"
#endif

void print(vec4 v) { printf("%.3f, %.3f, %.3f %.3f\n", v.x, v.y, v.z, v.w); }
void print(vec3 v) { printf("%.3f, %.3f, %.3f\n", v.x, v.y, v.z); }
void print(vec2 v) { printf("%.3f, %.3f\n", v.x, v.y); }

void print_ref(const vec2& v)
{
	printf("Printing ref: ");
	print(v);
}

void modify_ref(vec2& v)
{
	v.x = 1337;
	v.y = 42;
}

TEST_CASE("Basic vector ops")
{
	vec3 a(1, 2, 3);
	vec3 b(4, 4, 4);

	CHECK(a == a);
	CHECK(a == vec3(1, 2, 3));
	CHECK(b == b);
	CHECK(b == vec3(4, 4, 4));

	CHECK(a + a == vec3(2, 4, 6));
	CHECK(a + b == vec3(5, 6, 7));

	vec2 c(1, 2);
	vec2 d(4, 4);

	CHECK(c == c);
	CHECK(c == vec2(1, 2));
	CHECK(vec2(1, 2) == c);
	CHECK(c + d == vec2(5, 6));
	CHECK(vec2(5, 6) == c + d);
}

TEST_CASE( "Vector swizzles" )
{
	vec3 a(1, 2, 3);
	vec2 b(0, 0);

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
	// print_ref(a.xy);
	a.xz += vec2(1,1);
	CHECK(a == vec3(2,1,2));

	a.z = 1;
	a.xy += a.zz;
	CHECK(a == vec3(3,2,1));

	vec4 d(1, 2, 3, 4);
	
	a = vec3(1, 2, 3);
	CHECK(a == d.xyz);

	d += 1;
	CHECK(d == vec4(2, 3, 4, 5));
	CHECK(d.xyz == a + 1);
}

// https://github.com/polymonster/maths
TEST_CASE("Polymonster examples")
{
	vec4 v(1, 2, 3, 4);

    vec4 swizz = v.wzyx;        // construct from swizzle
    CHECK(swizz == vec4(4, 3, 2, 1));

    swizz = v.xxxx;             // assign from swizzle
    CHECK(swizz == vec4(1));

    swizz.wyxz = v.xxyy;        // assign swizzle to swizzle
    CHECK(swizz == vec4(2, 1, 2, 1));

    vec2 v2 = swizz.yz;         // construct truncated
    CHECK(v2 == vec2(1, 2));

    swizz.wx = v.xy;            // assign truncated
    CHECK(swizz.w == v.x);
    CHECK(swizz.x == v.y);

    swizz = vec4(1, 2, 3, 4);
    vec4 swizz2(2, 2, 2, 2);
    swizz.xyz *= swizz2.www;    // arithmetic on swizzles
    CHECK(swizz.xyz == vec3(2, 4, 6));

    v2 = swizz.xy * 2.0f;  		// swizzle / scalar arithmetic
    CHECK(v2 == vec2(4, 8));
}

TEST_CASE("non-assignment operators on swizzles")
{
	vec3 a = vec3(3,2,1);
	vec2 xy(a.xy);
	vec2 yx(a.yx);
	vec2 sum(5,5);

	CHECK(a.xy == a.xy);
	CHECK(a.xy == xy);
	CHECK(xy == a.xy);

	CHECK(a.xy + yx == sum);
	CHECK(xy + a.yx == sum);
	CHECK(a.xy + a.yx == sum);

	CHECK(sum == a.xy + yx);
	CHECK(sum == xy + a.yx);
	CHECK(sum == a.xy + a.yx);

	vec2 xz(a.x, a.z);
	vec2 xz2;
	xz2 = a.xz;

	CHECK(xz2 == xz);
	CHECK((vec2)a.xz == xz);
	CHECK(a.xz == xz);
	CHECK(xz == a.xz);
	CHECK(xz == (vec2)a.xz);

	CHECK(1 + a.xy == xy + 1);
	CHECK(a.xy + 1 == xy + 1);
	xy += 1;
	CHECK(1 + a.xy == xy);
	CHECK(a.xy + 1 == xy);
}

TEST_CASE("Assign between swizzles and vectors")
{
	vec3 a(1, 2, 3);
	vec2 b(a.x, a.y);
	vec2 c(0, 0);

	c = a.xy;
	CHECK(c == b);
	c = vec2(1,1);
	a.xy = c;
	CHECK(a.xy == c);
}

TEST_CASE("Assign between swizzles and splats")
{
	vec3 a(1, 2, 3);
	vec3 b(4, 5, 6);

	a.xy = a.zz;
	CHECK(a.xy == vec2(3, 3));
	CHECK(a.xy == a.zz);

	a.zyx = b;
	CHECK(a == vec3(6, 5, 4));
	CHECK(a == b.zyx);
	CHECK(a.zyx == b);

	//a.zz = b.xy; // This shuold NOT compile
	a.zz = b.zz; // Odd, but harmless?

	vec2 c(1, 2);

	// self assign swizzle
	c.xy = c.xy;
	CHECK(c == vec2(1, 2));
	c.yx = c.yx;
	CHECK(c == vec2(1, 2));

	// do ops with self
	c.xy = c.yx;
	CHECK(c == vec2(2, 1));
	c.yx = c.xy;
	CHECK(c == vec2(1, 2));
	c.yx = c.xy;
	CHECK(c == vec2(2, 1));
	c += 1;
	c.xy = c.yx;
	CHECK(c == vec2(2, 3));
	c.yx *= c.xy;
	CHECK(c == vec2(6, 6));
}

TEST_CASE("Splat arithmetic")
{
	vec3 a(1, 2, 3);
	vec3 b(4, 5, 6);

	vec3 c = a.xxx + b;
	CHECK(c == vec3(5, 6, 7));

	c = a + b.yyy;
	CHECK(c == vec3(6, 7, 8));

	c = a.xxx + b.yyy;
	CHECK(c == vec3(6, 6, 6));
	CHECK(c.xxx == c.yyy);
	CHECK(c.xxx != a.xxx);

	c = -a.xxx;
	CHECK(c == vec3(-1, -1, -1));
}

TEST_CASE("Vector Vector and Vector Scalar")
{	
	vec3 a(1, 2, 3);

	CHECK(a + 1 == vec3(2, 3, 4));
	CHECK(a * 2 == vec3(2, 4, 6));
	CHECK(1 + a == vec3(2, 3, 4));
	CHECK(2 * a == vec3(2, 4, 6));

	a += 1;
	CHECK(a == vec3(2, 3, 4));
	a *= 2;
	CHECK(a == vec3(4, 6, 8));
}

TEST_CASE("non-assignment operators on splats")
{
	vec3 a = vec3(3,2,1);
	vec2 xx(a.xx);
	vec2 yy(a.yy);
	vec2 sum(5,5);

	CHECK(a.xx == a.xx);
	CHECK(a.xx == xx);
	CHECK(xx == a.xx);

	CHECK(a.xx + yy == sum);
	CHECK(xx + a.yy == sum);
	CHECK(a.xx + a.yy == sum);

	CHECK(sum == a.xx + yy);
	CHECK(sum == xx + a.yy);
	CHECK(sum == a.xx + a.yy);
}

TEST_CASE("free functions")
{
	vec3 a(1,2,3);
	vec2 xx(a.xx);
	vec2 xy(a.xy);
	vec2 yz(a.yz);

	CHECK(dot(xy, xy) == 5.0f	);
	CHECK(dot(a.xy, a.xy) == 5.0f);
	CHECK(dot(a.xy, a.yz) == dot(xy, yz));
	CHECK(dot(a.xx, a.xx) == dot(xx, xx));
}