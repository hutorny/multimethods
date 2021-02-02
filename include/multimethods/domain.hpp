#pragma once
#include <type_traits>
#include <array>
#include <utility>
#include <cstdint>

namespace multimethods {
namespace details {

template<class Class, class ... Classes, size_t ... Indices>
constexpr size_t index_of(std::index_sequence<Indices...>) noexcept {
  return ((std::is_same_v<Class, Classes>*(Indices+1)) + ...);
}

template<bool Found, class Class>
constexpr void check_if_found() noexcept  {
  static_assert(Found, "Class not found");
}

template<class Class, class ... Classes>
constexpr size_t index_of() noexcept {
  constexpr ssize_t onebased = index_of<Class, Classes...>(std::make_index_sequence<sizeof...(Classes)>{});
  check_if_found<onebased != 0, Class>();
  return onebased - 1;
}

template<class First, class Second>
constexpr bool abstract_before() noexcept  {
  constexpr bool abstract_class_goes_before = std::is_abstract_v<First> >= std::is_abstract_v<Second>;
  static_assert(abstract_class_goes_before, "Abstract class goes after non abstract");
  return abstract_class_goes_before;
}
template<size_t ... Indices>
constexpr size_t first_nozero_index() noexcept {
  constexpr std::array<size_t,sizeof...(Indices)> indices { Indices ... };
  for(auto i : indices) if (i!=0) return i;
  return 0;
}
template<class ... Classes>
struct abstract_classes {
  template<class ... Others>
  static constexpr bool go_before() noexcept {
    return (abstract_before<Classes, Others>() && ...);
  }
  template<size_t ... Index>
  static constexpr size_t skip_till_concrete(std::index_sequence<Index...>) noexcept {
    constexpr auto first_nz = first_nozero_index<((!std::is_abstract_v<Classes>)*Index)...>();
    return first_nz;
  }
};
template<class Class, class ... Classes>
struct abstract_order {
  struct Tail {};
  static constexpr bool valid = abstract_classes<Class, Classes...>::template go_before<Classes..., Tail>();
};

template<class Class, class ... Classes>
constexpr size_t find_first_concrete() noexcept {
  struct Tail {};
  if constexpr(sizeof...(Classes) == 0 or not std::is_abstract_v<Class>) return 0;
  else {
    constexpr auto result = abstract_classes<Class, Classes...>::template
           skip_till_concrete(std::make_index_sequence<1+sizeof...(Classes)>{});
    return result;
  }
}

}
/*
   domain template - an utility for automatic assignment of class identifiers,
   ensuring order of abstract-concrete class and querying first concrete class
   Usage:
     1. forward-declare all classes
     2. use those classes as parameters for the template
           using MyDomain = multimethods::domain< MyClass1, MyClass2, ... >;
     3. in the class definition use template id_of<MyClass1> to get an ID for
        a class from the list:
           static constexpr auto classid = MyDomain::id_of<MyClass1>();
     4. place MyDomain::validate() in a C++ source, included in the build
*/
template<class ... Classes>
struct domain {
  static constexpr auto size = sizeof...(Classes);
  static constexpr auto first_concrete() noexcept {
	  constexpr auto result = details::find_first_concrete<Classes...>();
	  return result;
  }
  template<class Class>
  static constexpr size_t id_of() noexcept {
    constexpr auto value = details::index_of<Class, Classes...>();
    return value;
  }
  template<size_t ID>
  using type = std::tuple_element_t<ID, std::tuple<Classes...>>;
  static constexpr bool validate() noexcept {
    if constexpr(size <= 1) return true;
    else {
      constexpr bool order = details::abstract_order<Classes...>::valid;
      return order;
    }
  }
};
}
