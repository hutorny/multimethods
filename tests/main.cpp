#include <string>
#include <array>
#include <functional>
#include <chrono>

#include <multimethods/domain.hpp>
#include "shapes.hpp"
#include "calculus.hpp"
#include "perf.hpp"

namespace domaintest {
  void test();
}

static int loop_count(int argc, char** argv) {
  if(argc > 1) return std::stoi(argv[1]);
  return 0;
}

// multimethod templates contain provisioning for print_diagonstics.
// The users are expected to to provide implementation for those methods when needed
// This section illustrates how to specialize them on a per-use basis
namespace multimethods {
namespace details {
template<class ClassA, class ClassB, auto ... Entries>
template<class Stream>
void function_scores<ClassA, ClassB, Entries...>::print_diagonstics(Stream& out) const {
  const char *dlm = "";
  for(const auto&i : scores)
    if(i.value != 0) {
      out << dlm << i;
      dlm = ", ";
    }
}
} //namespace

template<>
template<size_t A, size_t B>
inline void perf2::mdispatch::print_diagonstics(size_t idA, size_t idB) {
  constexpr scores_type<A,B> scores {};
  std::cout << "dispatch failure: ";
  ::print_pair(std::cout, idA, idB);
  ::print_pair(std::cout << ":", A , B) << "=>";
  scores.print_diagonstics(std::cout);
  std::cout << "max :" << scores.highest() << std::endl;
}
} //namespace

namespace perf3 {

void test() {
  for(size_t x = 5; x < 25; x++) {
    for(size_t y = 5; y < 25; y++) {
#ifdef DEBUG
      print_pair(std::cout, x, y) << "->";
#endif
      subv(instance(x),instance(y));
    }
  }
}

void test(int loops) {
  std::random_device dev{};
  std::default_random_engine eng{dev()};
  std::uniform_int_distribution<size_t> urand(domain::first_concrete(), domain::size-1);
  while(--loops>=0) {
     size_t x = urand(eng);
     size_t y = urand(eng);
#ifdef DEBUG
     print_pair(std::cout, x, y) << "->";
#endif
     subv(instance(x),instance(y));
  }
}
}

namespace perf4 {
using namespace perf2;

void test() {
  for(size_t x = 5; x < 25; x++) {
    for(size_t y = 5; y < 25; y++) {
#ifdef DEBUG
      print_pair(std::cout, x, y) << "->";
#endif
      subx(instance(x),instance(y));
    }
  }
}

void test(int loops) {
  std::random_device dev{};
  std::default_random_engine eng{dev()};
  std::uniform_int_distribution<size_t> urand(domain::first_concrete(), domain::size-1);
  while(--loops>=0) {
     size_t x = urand(eng);
     size_t y = urand(eng);
#ifdef DEBUG
     print_pair(std::cout, x, y) << "->";
#endif
     subx(instance(x),instance(y));
  }
}
}
int main(int argc, char** argv) {
  int loops = loop_count(argc, argv);
  if(loops==0) {
    std::cout << "Running test examples" << std::endl;
    //perf2::mdispatch::print_diagonstics<24,24>(24,24);
    std::cout << "--------------  domaintest test ---------------\n" << std::flush;
    domaintest::test();
    std::cout << "--------------    shapes test   ---------------\n" << std::flush;
    shapes::test();
    std::cout << "--------------   calculus test  ---------------\n" << std::flush;
    calculus::test();
    std::cout << "--------------    perf1 test    ---------------\n" << std::flush;
    perf1::test();
    std::cout << "--------------    perf2 test    ---------------\n" << std::flush;
    perf2::test();
    std::cout << "--------------    visit test    ---------------\n" << std::flush;
    perf3::test();
    std::cout << "--------------    visit* test   ---------------\n" << std::flush;
    perf4::test();
  } else {
    std::cout << "Running performance test for " << loops << " loops" << std::endl;
    std::cout << "perf1 multimethod test   : " << std::flush;
    auto start = std::chrono::system_clock::now();
    perf1::test(loops);
    std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - start;
    std::cout << elapsed.count() << "sec\n";
    std::cout << "perf2 matrixdispatch test: " << std::flush;
    start = std::chrono::system_clock::now();
    perf2::test(loops);
    elapsed = std::chrono::system_clock::now() - start;
    std::cout << elapsed.count() << "sec\n";
    std::cout << "perf3 std::visit test: " << std::flush;
    start = std::chrono::system_clock::now();
    perf3::test(loops);
    elapsed = std::chrono::system_clock::now() - start;
    std::cout << elapsed.count() << "sec\n";
    std::cout << "perf4 std::visit* test: " << std::flush;
    start = std::chrono::system_clock::now();
    perf4::test(loops);
    elapsed = std::chrono::system_clock::now() - start;
    std::cout << elapsed.count() << "sec\n";
  }
  return 0;
}
