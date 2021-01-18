#include "perf.hpp"

namespace perf {
size_t add(const Base& a, const Base&b) {
	return multimethod<size_t,
		addi<Class<24>,Class< 6>>,
		addi<Class<24>,Class< 5>>,
		addi<Class<24>,Class< 4>>,
		addi<Class<22>,Class<22>>,
		addi<Class<22>,Class< 3>>,
		addi<Class<22>,Class< 2>>,
		addi<Class<20>,Class<22>>,
		addi<Class<20>,Class<12>>,
		addi<Class<21>,Class<15>>,
		addi<Class<21>,Class<14>>,
		addi<Class<20>,Class<13>>,
		addi<Class<20>,Class<12>>,
		addi<Class<19>,Class<16>>,
		addi<Class<18>,Class<15>>,
		addi<Class<18>,Class<14>>,
		addi<Class<17>,Class<13>>,
		addi<Class<17>,Class<12>>,
		addi<Class<16>,Class<11>>,
		addi<Class<16>,Class<10>>,
		addi<Class<15>,Class<15>>,
		addi<Class<14>,Class<14>>,
		addi<Class<14>,Class<13>>,
		addi<Class<13>,Class<13>>,
		addi<Class<12>,Class<12>>,
		addi<Class<11>,Class<11>>,
		addi<Class<10>,Class< 4>>,
		addi<Class< 9>,Class< 3>>,
		addi<Class< 9>,Class< 3>>,
		addi<Class< 8>,Class< 2>>,
		addi<Class< 8>,Class< 2>>,
		addi<Class< 7>,Class<11>>,
		addi<Class< 7>,Class<12>>,
		addi<Class< 7>,Class<10>>,
		addi<Class< 6>,Class<16>>,
		addi<Class< 6>,Class< 6>>,
		addi<Class< 5>,Class<15>>,
		addi<Class< 5>,Class< 5>>,
		addi<Class< 5>,Class< 1>>,
		addi<Class< 4>,Class< 4>>,
		addi<Class< 4>,Class< 1>>,
		addi<Class< 3>,Class< 3>>,
		addi<Class< 2>,Class< 2>>,
		addi<Class< 1>,Class< 1>>
     >::dispatch(a, b);
}

constexpr pair pairs[] = {
	{24, 6},
	{24, 5},
	{24, 4},
	{24,20},
	{24, 4},
	{22,22},
	{22,13},
	{22,12},
	{22, 4},
	{22, 3},
	{22, 2},
	{22, 1},
	{21,15},
	{21,14},
	{21, 5},
	{21, 4},
	{20,13},
	{20,12},
	{19,16},
	{18,15},
	{18,14},
	{17,13},
	{17,12},
	{16,11},
	{16,10},
	{15,15},
	{14,14},
	{14,13},
	{13,13},
	{12,12},
	{11,11},
	{11, 2},
	{10, 4},
	{ 9, 3},
	{ 9, 3},
	{ 8, 2},
	{ 8, 2},
	{ 7,11},
	{ 7,12},
	{ 7,10},
	{ 6,16},
	{ 6, 6},
	{ 5,15},
	{ 5, 5},
	{ 4, 4},
	{ 3, 3},
	{ 2, 2},
	{ 1, 1}
};

pair get(unsigned index) {
	return pairs[index % std::extent_v<decltype(pairs)>];
}

std::tuple<
    Class< 0>,
	Class< 1>,
	Class< 2>,
	Class< 3>,
	Class< 4>,
	Class< 5>,
	Class< 6>,
	Class< 7>,
	Class< 8>,
	Class< 9>,
	Class<10>,
	Class<11>,
	Class<12>,
	Class<13>,
	Class<14>,
	Class<15>,
	Class<16>,
	Class<17>,
	Class<18>,
	Class<19>,
	Class<20>,
	Class<21>,
	Class<22>,
	Class<23>,
	Class<24>> storage {};

Base& instance(unsigned N) {
  switch(N) {
  case  1: return std::get< 1>(storage);
  case  2: return std::get< 2>(storage);
  case  3: return std::get< 3>(storage);
  case  4: return std::get< 4>(storage);
  case  5: return std::get< 5>(storage);
  case  6: return std::get< 6>(storage);
  case  7: return std::get< 7>(storage);
  case  8: return std::get< 8>(storage);
  case  9: return std::get< 9>(storage);
  case 10: return std::get<10>(storage);
  case 11: return std::get<11>(storage);
  case 12: return std::get<12>(storage);
  case 13: return std::get<13>(storage);
  case 14: return std::get<14>(storage);
  case 15: return std::get<15>(storage);
  case 16: return std::get<16>(storage);
  case 17: return std::get<17>(storage);
  case 18: return std::get<18>(storage);
  case 19: return std::get<19>(storage);
  case 20: return std::get<20>(storage);
  case 21: return std::get<21>(storage);
  case 22: return std::get<22>(storage);
  case 23: return std::get<23>(storage);
  case 24: return std::get<24>(storage);
  default:
	  LOGIC_ERROR("Out of bounds");
  }
}

}
