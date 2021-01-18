#include "shapes.hpp"
namespace shapes {

void test(const Shape& a, const Shape& b) {
  a.intersect(b);
  b.join(a);
}

} // namespace shapes
