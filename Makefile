.PHONY: test test_templates
default: test test_templates

CXXFLAGS:=-std=c++11 -Wall -Wextra -fsanitize=undefined

main: main.cpp
	$(CXX) main.cpp $(CXXFLAGS) -o main

main_templates: main_templates.cpp
	$(CXX) main_templates.cpp $(CXXFLAGS) -o main_templates

test: test.cpp
	$(CXX) test.cpp $(CXXFLAGS) -o test
	./test

test_templates: test.cpp
	$(CXX) test.cpp $(CXXFLAGS) -std=c++20 -DVEC_TEMPLATES -o test_templates
	./test_templates