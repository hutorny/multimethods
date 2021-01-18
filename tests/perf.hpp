#pragma once
#include <iostream>
#include <tuple>
#include <utility>
#include <multimethods/multimethods.hpp>
#include <multimethods/classinfo.hpp>
#include <multimethods/resolve.hpp>

#ifdef DEBUG
#define DTRACE() \
  std::cout << __PRETTY_FUNCTION__ << std::endl;
#else
#define DTRACE()
#endif

namespace perf {
using namespace multimethods;
template<unsigned N>
class Class : public Class<N/5> {
public:
  static constexpr auto classid = multimethods::class_hash<Class>();
  virtual ~Class() {}
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

using Base = Class<0>;
size_t add(const Base& a, const Base& b);

template<class A, class B>
size_t addi(const A& a, const B& b) {
	DTRACE();
	return a.classid + b.classid;
}

struct pair {
	unsigned value[2];
};

Base& instance(unsigned N);
pair get(unsigned index);

inline void test(unsigned x, unsigned y) {
#ifdef DEBUG
	std::cout << "test(" << x << "," << y << ")\n";
#endif
	const auto& a = instance(x);
	const auto& b = instance(y);
	add(a,b);
}

inline void test(int loops) {
  while(--loops>=0)
	 for(unsigned i = 0; i < 200; i++) {
		auto [x,y] = get(i).value;
		test(x,y);
	}
}

}
