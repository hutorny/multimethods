#include "perf.hpp"
namespace perf3 {
using namespace perf2;
struct overloaded {
  size_t operator()(Class<24>& a, Class<24>& b) const noexcept { return a.sub(b); }
  size_t operator()(Class<24>& a, Class<23>& b) const noexcept { return a.sub(b); }
  size_t operator()(Class<24>& a, Class<21>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<23>& a, Class<21>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<22>& a, Class<22>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<21>& a, Class<12>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<20>& a, Class<15>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<19>& a, Class<16>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<18>& a, Class<15>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<17>& a, Class<13>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<16>& a, Class<11>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<15>& a, Class<15>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<14>& a, Class<14>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<13>& a, Class<13>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<12>& a, Class<12>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<11>& a, Class<11>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<10>& a, Class<14>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 9>& a, Class<12>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 8>& a, Class<12>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 7>& a, Class<11>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 6>& a, Class<16>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 5>& a, Class<15>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 5>& a, Class< 5>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 5>& a, Class< 2>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 5>& a, Class< 1>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 4>& a, Class< 4>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 4>& a, Class< 3>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 4>& a, Class< 2>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 4>& a, Class< 1>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 3>& a, Class< 4>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 3>& a, Class< 3>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 3>& a, Class< 2>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 3>& a, Class< 1>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 2>& a, Class< 4>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 2>& a, Class< 3>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 2>& a, Class< 2>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 2>& a, Class< 1>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 1>& a, Class< 4>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 1>& a, Class< 3>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 1>& a, Class< 2>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class< 1>& a, Class< 1>& b) const noexcept { return sub(a,b); }
  size_t operator()(Class<0>& a, Class<0>& b) const noexcept   { return sub(a,b); }
};


size_t subv(ClassV& a, ClassV& b) {
  return std::visit(overloaded{},a,b);
  //return 0;
}
Owner owner;
template<size_t N>
static ClassV local{std::in_place_type_t<Class<N>>{}, owner};

template<size_t N>
ClassV& instance() {
  return local<N>;
}

ClassV& instance(unsigned N) {
  switch(N) {
  case  5: return instance< 5>();
  case  6: return instance< 6>();
  case  7: return instance< 7>();
  case  8: return instance< 8>();
  case  9: return instance< 9>();
  case 10: return instance<10>();
  case 11: return instance<11>();
  case 12: return instance<12>();
  case 13: return instance<13>();
  case 14: return instance<14>();
  case 15: return instance<15>();
  case 16: return instance<16>();
  case 17: return instance<17>();
  case 18: return instance<18>();
  case 19: return instance<19>();
  case 20: return instance<20>();
  case 21: return instance<21>();
  case 22: return instance<22>();
  case 23: return instance<23>();
  case 24: return instance<24>();
  default:
    LOGIC_ERROR("Out of bounds");
  }
}
}
namespace perf4 {
using namespace perf2;

struct funclist {
  size_t operator()(Class<24>* a, Class<24>* b) const noexcept { return a->sub(*b); }
  size_t operator()(Class<24>* a, Class<23>* b) const noexcept { return a->sub(*b); }
  size_t operator()(Class<24>* a, Class<21>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<23>* a, Class<21>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<22>* a, Class<22>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<21>* a, Class<12>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<20>* a, Class<15>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<19>* a, Class<16>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<18>* a, Class<15>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<17>* a, Class<13>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<16>* a, Class<11>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<15>* a, Class<15>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<14>* a, Class<14>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<13>* a, Class<13>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<12>* a, Class<12>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<11>* a, Class<11>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<10>* a, Class<14>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 9>* a, Class<12>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 8>* a, Class<12>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 7>* a, Class<11>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 6>* a, Class<16>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 5>* a, Class<15>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 5>* a, Class< 5>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 5>* a, Class< 2>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 5>* a, Class< 1>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 4>* a, Class< 4>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 4>* a, Class< 3>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 4>* a, Class< 2>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 4>* a, Class< 1>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 3>* a, Class< 4>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 3>* a, Class< 3>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 3>* a, Class< 2>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 3>* a, Class< 1>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 2>* a, Class< 4>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 2>* a, Class< 3>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 2>* a, Class< 2>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 2>* a, Class< 1>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 1>* a, Class< 4>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 1>* a, Class< 3>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 1>* a, Class< 2>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class< 1>* a, Class< 1>* b) const noexcept { return sub(*a,*b); }
  size_t operator()(Class<0>* a, Class<0>* b) const noexcept   { return sub(*a,*b); }
};

size_t subx(Class<0>& a, Class<0>&b) {
  return std::visit(funclist{}, a.varpointer(), b.varpointer());
}

}
