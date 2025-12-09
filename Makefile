.PHONY: test test_templates
default: test test_templates

CXXFLAGS:=-Wall -Wextra -fsanitize=undefined

main: main.cpp
	$(CXX) main.cpp $(CXXFLAGS) -o main

main_templates: main_templates.cpp
	$(CXX) main_templates.cpp $(CXXFLAGS) -o main_templates

test: test.cpp
	time $(CXX) test.cpp $(CXXFLAGS) -std=c++11 -o test
	time ./test

test_templates: test.cpp
	time $(CXX) test.cpp $(CXXFLAGS) -std=c++20 -DVEC_TEMPLATES -o test_templates
	time ./test_templates