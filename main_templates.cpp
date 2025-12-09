#include "vec_templates.h"

int main()
{
	vec3 a(1.0f);
	vec3 b(1.0f, 2.0f, 3.0f);

	return (int)(a + b).length();
}