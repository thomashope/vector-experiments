This is a test repo where I was messing around with impleminting vector swizzles without using templates.

Why no templates? People say they are slow to compile + link, and can lead to rather slow code, particularly in debug builds. I also have a suspicion templates hinder the Edit and Continue button in Visual Studio which IMO is great when it works.

This implementation uses some macros, a code gen script, and the 'common initial sequence' exemption in unions to avoid undefined behaviour. Lets see how it turns out.

Generate the swizzle files with `ruby generate_swizzles.rb`.

Swizzle implementations are generated from macros inside `vec.h`.

Take a look in `test.cpp` to see how to use the swizzles.

Swizzles can give nice ergonomics, but often come with drawbacks. This is an experiment too examine those drawbacks a bit more cloesly and see if they can be mitigated with an 'old school' approach vs the templated implementations i've seen before.

TODO:
- [ ] implement enough stuff in the macro version so I can do a fair comparison
	- all ops for vec2, 3, and 4
- [ ] bring in a comparable template based implementation to test against
	- features. Is any differences in what the implementations are capable of?
	- compiler compatability / what level of C++ features is required
	- compile times in debug and release
	- binary size
	- performance
	- see if they work with Edit and Continue?
	- debugging
		- setting breakpoints
		- how they look in the debug window


inspo
- https://madethisthing.com/iq/vectypes