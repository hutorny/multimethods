#include <string>

#include "shapes.hpp"
#include "calculus.hpp"
#include "perf.hpp"

int loop_count(int argc, char** argv) {
	if(argc > 1) return std::stoi(argv[1]);
	return 1;
}


int main(int argc, char** argv) {
	int loops = loop_count(argc, argv);
    shapes::test();
    calculus::test();
    std::cout << "Running perfromance test for " << loops << " loops" << std::endl;
    perf::test(loops);
    return 0;
}
