CXXFLAGS=-I../include -std=c++17 -O3 -fno-rtti -fno-exceptions -Wall -Wextra -pg

test : main.o calculus.o perf.o shapes.o
	$(CXX) -pg -o $@ $^

clean:
	$(RM) test *.o