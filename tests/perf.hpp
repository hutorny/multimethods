#pragma once
#include <iostream>
#include <iomanip>
#include <tuple>
#include <utility>
#include <random>
#include <variant>

#include <multimethods/multimethods.hpp>
#include <multimethods/classinfo.hpp>
#include <multimethods/resolve.hpp>

#ifdef DEBUG
#define DTRACE() \
  std::cout << __PRETTY_FUNCTION__ << std::endl;
#else
#define DTRACE()
#endif

template<typename A, typename B>
std::ostream& print_pair(std::ostream& out, A&& a, B&& b) {
	return out << '{' << a << ',' << b << '}';
}

inline std::ostream& operator<<(std::ostream& out, const multimethods::details::function_score& val) {
  return print_pair(out, val.index, val.value);
}

namespace perf1 {
// Sanity and Performance tests for linear multimethod::dispatch
using namespace multimethods;
struct Owner;

template<unsigned N>
class Class : public Class<N/5> {
public:
  static constexpr auto classid = multimethods::class_hash<Class>();
  virtual ~Class() {}
  Class(const Owner& owner) : Class<N/5> {owner} {}
protected:
  using base = Class<N/5>;
  virtual bool instance_of(const class_info& expected) const noexcept {
  return classinfo(*this) == expected or base::instance_of(expected);
  }
};

template<>
class Class<0> {
public:
  static constexpr auto classid = multimethods::class_hash<Class>();
  Class(const Owner&)  {}
  // Delete constructors ensures there are no copies created
  Class() = delete;
  Class(const Class&) = delete;
  Class(Class&&) = delete;
  Class& operator=(const Class&)=delete;
  template<class Expected>
  bool instanceof() const noexcept {
  return instance_of(classinfo<Expected>());
  }
  virtual ~Class() {}
protected:
  virtual bool instance_of(const class_info& expected) const noexcept {
  return classinfo(*this) == expected;
  }
};

size_t add(const Class<0>& a, const Class<0>& b);

template<class A, class B>
size_t addi(const A& a, const B& b) {
  DTRACE();
  return a.classid + b.classid;
}

Class<0>& instance(unsigned N);

inline void test(size_t x, size_t y) {
#ifdef DEBUG
  std::cout << "test(" << std::setw(2) << x << "," << std::setw(2) << y << ") => ";
#endif
  const auto& a = instance(x);
  const auto& b = instance(y);
  add(a,b);
}

inline void test() {
  std::cout << "======= perf1 example ========\n";
  for(unsigned x = 5; x < 24; x++) {
    for(unsigned y = 5; y < 24; y++) {
      test(x,y);
    }
  }
}

inline void test(int loops) {
  std::random_device dev{};
  std::default_random_engine eng{dev()};
  std::uniform_int_distribution<size_t> urand(5, 24);
  while(--loops>=0) {
    test(urand(eng),urand(eng));
  }
}
}
//--------------------------------------------------------------------------------
namespace perf2 {
// Sanity and Performance tests for matrixdispatch::dispatch
class Owner {};

template<size_t N>
class Class;

using varptr = std::variant<
    Class<0>*,  Class<1>*,  Class<2>*,  Class<3>*,  Class<4>*,
    Class<5>*,  Class<6>*,  Class<7>*,  Class<8>*,  Class<9>*,
    Class<10>*, Class<11>*, Class<12>*, Class<13>*, Class<14>*,
    Class<15>*, Class<16>*, Class<17>*, Class<18>*, Class<19>*,
    Class<20>*, Class<21>*, Class<22>*, Class<23>*, Class<24>*
>;


template<size_t N>
class Class : public Class<N/5> {
public:
  static constexpr auto classid = N;
  // Owner here is a dummy parameter just to have one
  Class(const Owner& owner) : Class<N/5> {owner}{}
  size_t classID() const noexcept override {
    return classid;
  }
  varptr varpointer() noexcept override {
    return varptr { std::in_place_type_t<Class<N>*>{}, this };
  }
};

template<>
class Class<0> {
public:
  static constexpr auto classid = 0;
  Class(const Owner&) {}
  // Delete constructors ensures there are no copies created
  Class() = delete;
  Class(const Class&) = delete;
  Class(Class&&) = delete;
  Class& operator=(const Class&)=delete;
  virtual varptr varpointer() noexcept = 0;
  virtual ~Class() {}
  virtual size_t classID() const noexcept {
    return classid;
  }
};

template<>
struct Class<24> : Class<4> {
  static constexpr auto classid = 24;
  Class(const Owner& owner) : Class<classid/5> {owner}{}
  template<class B>
  size_t sub(B& b) const {
    DTRACE();
    return classid + b.classID();
  }
  size_t classID() const noexcept override {
    return classid;
  }
  varptr varpointer() noexcept override {
    return varptr { std::in_place_type_t<Class<24>*>{}, this };
  }
};

Class<0>& instance(unsigned N);

struct domain {
  static constexpr size_t size = 25;
  static constexpr size_t first_concrete() noexcept {
    return 5;
  }
  template<size_t N>
  using type = Class<N>;
};

template<class A, class B>
size_t sub(A& a, B& b) {
  DTRACE();
  return a.classID() + b.classID();
}


size_t subd(Class<0>& a, Class<0>& b);

using mdispatch = multimethods::matrixdispatch<decltype(&subd),domain,domain,
  &Class<24>::sub<Class<24>>,
  &Class<24>::sub<Class<23>>,
  sub<Class<24>,Class<21>>,
  sub<Class<23>,Class<21>>,
  sub<Class<22>,Class<22>>,
  sub<Class<21>,Class<12>>,
  sub<Class<20>,Class<15>>,
  sub<Class<19>,Class<16>>,
  sub<Class<18>,Class<15>>,
  sub<Class<17>,Class<13>>,
  sub<Class<16>,Class<11>>,
  sub<Class<15>,Class<15>>,
  sub<Class<14>,Class<14>>,
  sub<Class<13>,Class<13>>,
  sub<Class<12>,Class<12>>,
  sub<Class<11>,Class<11>>,
  sub<Class<10>,Class<14>>,
  sub<Class< 9>,Class<12>>,
  sub<Class< 8>,Class<12>>,
  sub<Class< 7>,Class<11>>,
  sub<Class< 6>,Class<16>>,
  sub<Class< 5>,Class<15>>,
  sub<Class< 5>,Class< 5>>,
  sub<Class< 5>,Class< 2>>,
  sub<Class< 5>,Class< 1>>,
  sub<Class< 4>,Class< 4>>,
  sub<Class< 4>,Class< 3>>,
  sub<Class< 4>,Class< 2>>,
  sub<Class< 4>,Class< 1>>,
  sub<Class< 3>,Class< 4>>,
  sub<Class< 3>,Class< 3>>,
  sub<Class< 3>,Class< 2>>,
  sub<Class< 3>,Class< 1>>,
  sub<Class< 2>,Class< 4>>,
  sub<Class< 2>,Class< 3>>,
  sub<Class< 2>,Class< 2>>,
  sub<Class< 2>,Class< 1>>,
  sub<Class< 1>,Class< 4>>,
  sub<Class< 1>,Class< 3>>,
  sub<Class< 1>,Class< 2>>,
  sub<Class< 1>,Class< 1>>
>;

inline void test() {
  for(size_t x = domain::first_concrete(); x < domain::size; x++) {
    for(size_t y = domain::first_concrete(); y < domain::size; y++) {
#ifdef DEBUG
      print_pair(std::cout, x, y) << "->";
#endif
      subd(instance(x),instance(y));
    }
  }
}
inline void test(int loops) {
  std::random_device dev{};
  std::default_random_engine eng{dev()};
  std::uniform_int_distribution<size_t> urand(domain::first_concrete(), domain::size-1);
  while(--loops>=0) {
     size_t x = urand(eng);
     size_t y = urand(eng);
#ifdef DEBUG
     print_pair(std::cout, x, y) << "->";
#endif
     subd(instance(x),instance(y));
  }
}
}
namespace perf3 {
using namespace perf2;

using ClassV = std::variant<
    Class<5>,  Class<6>,  Class<7>,  Class<8>,  Class<9>,
    Class<10>, Class<11>, Class<12>, Class<13>, Class<14>,
    Class<15>, Class<16>, Class<17>, Class<18>, Class<19>,
    Class<20>, Class<21>, Class<22>, Class<23>, Class<24>
>;
size_t subv(ClassV& a, ClassV&b);
ClassV& instance(unsigned N);
void test();
void test(int loops);
}

namespace perf4 {
using namespace perf2;
size_t subx(Class<0>& a, Class<0>&b);
}
