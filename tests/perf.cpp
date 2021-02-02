#include "perf.hpp"

namespace perf1 {
size_t add(const Class<0>& a, const Class<0>&b) {
	return multimethod<size_t,
		addi<Class<24>,Class<24>>,
		addi<Class<23>,Class<22>>,
		addi<Class<22>,Class<22>>,
		addi<Class<21>,Class<12>>,
		addi<Class<20>,Class<15>>,
		addi<Class<19>,Class<16>>,
		addi<Class<18>,Class<15>>,
		addi<Class<17>,Class<13>>,
		addi<Class<16>,Class<11>>,
		addi<Class<15>,Class<15>>,
		addi<Class<14>,Class<14>>,
		addi<Class<13>,Class<13>>,
		addi<Class<12>,Class<12>>,
		addi<Class<11>,Class<11>>,
		addi<Class<10>,Class<14>>,
		addi<Class< 9>,Class<12>>,
		addi<Class< 8>,Class<12>>,
		addi<Class< 7>,Class<11>>,
		addi<Class< 6>,Class<16>>,
		addi<Class< 5>,Class<15>>,
		addi<Class< 5>,Class< 5>>,
		addi<Class< 5>,Class< 2>>,
		addi<Class< 5>,Class< 1>>,
		addi<Class< 4>,Class< 4>>,
		addi<Class< 4>,Class< 3>>,
		addi<Class< 4>,Class< 2>>,
		addi<Class< 4>,Class< 1>>,
		addi<Class< 3>,Class< 4>>,
		addi<Class< 3>,Class< 3>>,
		addi<Class< 3>,Class< 2>>,
		addi<Class< 3>,Class< 1>>,
		addi<Class< 2>,Class< 4>>,
		addi<Class< 2>,Class< 3>>,
		addi<Class< 2>,Class< 2>>,
		addi<Class< 2>,Class< 1>>,
		addi<Class< 1>,Class< 4>>,
		addi<Class< 1>,Class< 3>>,
		addi<Class< 1>,Class< 2>>,
		addi<Class< 1>,Class< 1>>
     >::dispatch(a, b);
}

struct Owner {};
const Owner owner;

template<size_t N>
Class<N> local {owner};

Class<0>& instance(unsigned N) {
  switch(N) {
  case  1: return local< 1>;
  case  2: return local< 2>;
  case  3: return local< 3>;
  case  4: return local< 4>;
  case  5: return local< 5>;
  case  6: return local< 6>;
  case  7: return local< 7>;
  case  8: return local< 8>;
  case  9: return local< 9>;
  case 10: return local<10>;
  case 11: return local<11>;
  case 12: return local<12>;
  case 13: return local<13>;
  case 14: return local<14>;
  case 15: return local<15>;
  case 16: return local<16>;
  case 17: return local<17>;
  case 18: return local<18>;
  case 19: return local<19>;
  case 20: return local<20>;
  case 21: return local<21>;
  case 22: return local<22>;
  case 23: return local<23>;
  case 24: return local<24>;
  default:
	  LOGIC_ERROR("Out of bounds");
  }
}

}
namespace perf2 {
const Owner owner;

template<size_t N>
static Class<N> local{owner};

template<size_t N>
Class<0>& instance() {
  return local<N>;
}

Class<0>& instance(unsigned N) {
  switch(N) {
//  case  1: return instance< 1>();
//  case  2: return instance< 2>();
//  case  3: return instance< 3>();
//  case  4: return instance< 4>();
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
size_t subd(Class<0>& a, Class<0>& b) {
  return mdispatch::dispatch(a,b);
}
}
