#pragma once
#include <iostream>

#include <multimethods/multimethods.hpp>
#include <multimethods/classinfo.hpp>
#include <multimethods/resolve.hpp>

#define TRACE() \
  std::cout << __PRETTY_FUNCTION__ << std::endl;

namespace shapes {
using namespace multimethods;

class Shape {
public:
  static constexpr auto classid = multimethods::class_hash<Shape>();
  template<class Expected>
  bool instanceof() const noexcept {
    return instance_of(classinfo<Expected>());
  }
  Shape* intersect(const Shape&) const;
  Shape* join(const Shape&) const;
  virtual ~Shape() {}
protected:
  virtual bool instance_of(const class_info& expected) const noexcept {
    return classinfo(*this) == expected;
  }
};

class Circle;
class Rect;

class Rect : public Shape {
public:
  static constexpr auto classid = class_hash<Rect>();
  Shape* intersect(const Circle&) const { TRACE(); return nullptr; }
  Rect* intersect(const Rect&)const { TRACE(); return nullptr; }
  Shape* join(const Circle&) { TRACE(); return nullptr; }
  Shape* join(const Rect&) { TRACE(); return nullptr; }
protected:
  bool instance_of(const class_info& expected) const noexcept override {
    return classinfo(*this) == expected or Shape::instance_of(expected);
  }
};

class Circle : public Shape {
public:
  static constexpr auto classid = class_hash<Circle>();
  Shape* intersect(const Circle&) const { TRACE(); return nullptr; }
  Shape* intersect(const Rect&) const { TRACE(); return nullptr; }
  Shape* join(const Circle&) { TRACE(); return nullptr; }
  Shape* join(const Rect&) { TRACE(); return nullptr; }
protected:
  bool instance_of(const class_info& expected) const noexcept override {
    return classinfo(*this) == expected or Shape::instance_of(expected);
  }
};

struct Square : public Rect {
  static constexpr auto classid = class_hash<Square>();
protected:
  bool instance_of(const class_info& expected) const noexcept override {
    return classinfo(*this) == expected or Rect::instance_of(expected);
  }
};

inline Shape* Shape::intersect(const Shape& shape) const {
  return multimethod<Shape*,
    resolve<Shape*, const Circle&>{}(&Rect::intersect),
    resolve<Rect*, const Rect&>{}(&Rect::intersect),
    resolve<Shape*, const Circle&>{}(&Circle::intersect),
    resolve<Shape*, const Rect&>{}(&Circle::intersect)>::call(*this, shape);
}

inline Shape* Shape::join(const Shape& shape) const {
  return multimethod<Shape*,
    resolve<Shape*, const Circle&>{}(&Rect::join),
    resolve<Shape*, const Rect&>{}(&Rect::join),
    resolve<Shape*, const Circle&>{}(&Circle::join),
    resolve<Shape*, const Rect&>{}(&Circle::join)>::call(*this, shape);
}

void test(const Shape& a, const Shape& b);

inline void test() {
  Circle circle{};
  Rect rect{};
  Square square{};
  test(rect, circle);
  test(circle, square);
  test(circle, circle);
  test(square, rect);
}
} // namespace shapes

