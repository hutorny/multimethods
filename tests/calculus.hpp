#pragma once
#include <iostream>
#include <multimethods/multimethods.hpp>

#define TRACE() \
  std::cout << __PRETTY_FUNCTION__ << std::endl;

namespace calculus {
using namespace multimethods;

struct Expression {
  virtual ~Expression() {}
};

struct Constant : Expression {};
struct Integer : Constant {};
struct Float : Constant {};

template<class A, class B>
Expression* add(const A&, const B&);
template<class A, class B>
Expression* sub(const A&, const B&);

template<>
inline Expression* add<Integer, Integer>(const Integer&, const Integer&) {	TRACE(); return nullptr; }
template<>
inline Expression* add<Float, Float>(const Float&, const Float&) { TRACE();	return nullptr; }
template<>
inline Expression* add<Float, Integer>(const Float&, const Integer&) { TRACE();	return nullptr; }
template<>
inline Expression* add<Integer, Float>(const Integer&, const Float&) { TRACE();	return nullptr; }

template<>
inline Expression* sub<Integer, Integer>(const Integer&, const Integer&) {  TRACE(); return nullptr; }
template<>
inline Expression* sub<Float, Float>(const Float&, const Float&) { TRACE(); return nullptr; }
template<>
inline Expression* sub<Float, Integer>(const Float&, const Integer&) { TRACE(); return nullptr; }
template<>
inline Expression* sub<Integer, Float>(const Integer&, const Float&) { TRACE(); return nullptr; }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
template<>
inline Expression* add<Expression,Expression>(const Expression& a, const Expression& b) {
// These tests depends on RTTI
#if __cpp_rtti >= 199711
  return multimethod<Expression*,
    add<Integer, Integer>,
    add<Float, Integer>,
    add<Integer, Float>,
    add<Float, Float>>::dispatch(a,b);
#else
  return nullptr;
#endif
}

template<>
inline Expression* sub<Expression,Expression>(const Expression& a, const Expression& b) {
#if __cpp_rtti >= 199711
  return multimethod<Expression*,
    sub<Integer, Integer>,
    sub<Float, Integer>,
    sub<Integer, Float>,
    sub<Float, Float>>::dispatch(a,b);
#else
  return nullptr;
#endif
}

void test(const Expression& a, const Expression& b);

inline void test() {
  Integer a{};
  Integer b{};
  Float c{};
  test(a, b);
  test(c, b);
  test(a, c);
  test(c, c);
}

} // namespace calculus
