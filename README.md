This is a test repo where I was messing around with impleminting vector swizzles without using templates.

Why no templates? People say they are slow to compile + link, and can lead to rather slow code, particularly in debug builds. I also have a suspicion templates hinder the Edit and Continue button in Visual Studio which IMO is great when it works.

This implementation uses some macros, a code gen script, and the 'common initial sequence' exemption in unions to avoid undefined behaviour. Lets see how it turns out.

Take a look in `test.cpp` to see how swizzles can work.

They can give nice ergonomics, but impelementations often they come with a not insignificant cost. This is an experiment too examine those costs a bit more cloesly and see if the drawbacks can be mitigated with an 'old school' approach.

TODO:
- [ ] implement enough stuff in the macro version so I can do a fair comparison
	- all ops for vec2, 3, and 4
- [ ] bring in a comparable template based implementation to profile against
	- compile times in debug and release
	- binary size
	- performance
	- see if they work with Edit and Continue?


inspo
- https://madethisthing.com/iq/vectypes