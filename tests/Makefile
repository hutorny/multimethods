DBG_FLAGS =  $(if $(DEBUG),-DDEBUG=$(DEBUG),) $(if $(NDEBUG),-DNDEBUG=$(NDEBUG),)
CXXFLAGS=-I../include -std=c++17 -O3 -fno-rtti -fno-exceptions -Wall -Wextra $(DBG_FLAGS)

# VisualStudio flags: /std:c++17 /permissive- /I ..\include /O2

test : main.o calculus.o perf.o shapes.o domain.o stdvisit.o
	$(CXX) -o $@ $^

clean:
	$(RM) test *.o

check: test
	./test
