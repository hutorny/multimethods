#pragma once
#include <cstddef>
#include <type_traits>
#include <tuple>
#include <multimethods/logic_error.hpp>

namespace multimethods {
namespace details {
template<class Class>
struct is_virtual_parameter {
  static constexpr bool value =
       std::is_polymorphic_v<std::remove_reference_t<Class>> and
       std::is_reference_v<Class>;
};

template<class Class>
constexpr auto is_virtual_parameter_v = is_virtual_parameter<Class>::value;

template<typename Parameter, typename Argument>
struct is_assignable_parameter {
  static constexpr bool value = std::is_assignable_v<std::remove_const_t<Parameter>&, Argument>;
};

template<typename Parameter, typename Argument>
constexpr auto is_assignable_parameter_v = is_assignable_parameter<Parameter, Argument>::value;

template<class Class>
constexpr auto has_instanceof(int) noexcept -> decltype(&Class::template instanceof<Class> ,true) { return true; }

template<class Class>
constexpr auto has_instanceof(long) noexcept { return false; }

template<class Class>
constexpr auto get_first_concrete(int) noexcept -> decltype(Class::first_concrete()) { return Class::first_concrete(); }

template<class Class>
constexpr size_t get_first_concrete(long) noexcept { return 0; }

template<class To, class From>
To& down_cast(From& from) noexcept {
  static_assert(std::is_base_of_v<std::remove_reference_t<From>, std::remove_reference_t<To>>);
  return static_cast<To&>(from);
}


template<class Parameter>
struct expected {
  using parameter_type = std::remove_reference_t<std::remove_cv_t<Parameter>>;
  template<class Argument>
  static constexpr bool matches(Argument&& argument) noexcept {
    if constexpr(has_instanceof<parameter_type>(0)) {
      return argument.template instanceof<parameter_type>();
    } else {
#if __cpp_rtti >= 199711
      return typeid(Parameter) == typeid(argument);
#else
      static_assert(not is_virtual_parameter_v<Parameter>, "No class info available");
      return is_assignable_parameter_v<Parameter, Argument>;
#endif
    }
  }
};
template<auto Method>
struct entry;

template<class Return, class ... Parameter, Return (*Function)(Parameter...)>
struct entry<Function> {
  template<class ... Argument>
  static constexpr bool matches(const Argument& ... argument) noexcept {
    return (expected<Parameter>::matches(argument) and ...);
  }
  template<class ... Argument>
  static Return call(Argument& ... argument) noexcept(noexcept(Function)) {
    return (*Function)(down_cast<Parameter>(argument)...);
  }
};

template<class Target, class Return, class ... Parameter, Return (Target::*Method)(Parameter...)>
struct entry<Method> {
  template<class Object, class ... Argument>
  static constexpr bool matches(Object& obj, Argument& ... argument) noexcept {
    return expected<Target>::matches(obj) and (expected<Parameter>::matches(argument) and ...);
  }
  template<class Object, class ... Argument>
  static Return call(Object& target, Argument& ... argument) noexcept(noexcept(Method)) {
    return ((Target&)(target).*Method)(down_cast<Parameter>(argument)...);
  }
};

template<class Target, class Return, class ... Parameter, Return (Target::*Method)(Parameter...)const>
struct entry<Method> {
  template<class Object, class ... Argument>
  static constexpr bool matches(const Object& obj, Argument& ... argument) {
    return expected<Target>::matches(obj) and (expected<Parameter>::matches(argument) and ...);
  }
  template<class Object, class ... Argument>
  static Return call(const Object& target, Argument& ... argument) {
    return ((const Target&)(target).*Method)(down_cast<Parameter>(argument)...);
  }
};

template<typename Function>
struct function_traits;

template<class Return, class ... Parameter>
struct function_traits<Return (*)(Parameter...)> {
  using result_type = Return;
  using class_type = void;
  static constexpr bool is_method = false;
  static constexpr bool is_noexcept = false;
  static constexpr bool is_const_method = false;
  static constexpr size_t parameter_count = sizeof...(Parameter);
  template<size_t N>
  using nth_arg_type = std::tuple_element_t<N, std::tuple<Parameter...>>;
};

template<class Return, class ... Parameter>
struct function_traits<Return (*)(Parameter...)noexcept> {
  using result_type = Return;
  using class_type = void;
  static constexpr bool is_method = false;
  static constexpr bool is_noexcept = true;
  static constexpr bool is_const_method = false;
  static constexpr size_t parameter_count = sizeof...(Parameter);
  template<size_t N>
  using nth_arg_type = std::tuple_element_t<N, std::tuple<Parameter...>>;
};

template<class Return, class Class, class ... Parameter>
struct function_traits<Return (Class::*)(Parameter...)noexcept> {
  using result_type = Return;
  using class_type = Class;
  static constexpr bool is_method = true;
  static constexpr bool is_noexcept = true;
  static constexpr bool is_const_method = false;
  static constexpr size_t parameter_count = sizeof...(Parameter);
  template<size_t N>
  using nth_arg_type = std::tuple_element_t<N, std::tuple<Parameter...>>;
};

template<class Return, class Class, class ... Parameter>
struct function_traits<Return (Class::*)(Parameter...)> {
  using result_type = Return;
  using class_type = Class;
  static constexpr bool is_method = true;
  static constexpr bool is_noexcept = false;
  static constexpr bool is_const_method = false;
  static constexpr size_t parameter_count = sizeof...(Parameter);
  template<size_t N>
  using nth_arg_type = std::tuple_element_t<N, std::tuple<Parameter...>>;
};

template<class Return, class Class, class ... Parameter>
struct function_traits<Return (Class::*)(Parameter...)const noexcept> {
  using result_type = Return;
  using class_type = Class;
  static constexpr bool is_method = true;
  static constexpr bool is_noexcept = true;
  static constexpr bool is_const_method = true;
  static constexpr size_t parameter_count = sizeof...(Parameter);
  template<size_t N>
  using nth_arg_type = std::tuple_element_t<N, std::tuple<Parameter...>>;
};

template<class Return, class Class, class ... Parameter>
struct function_traits<Return (Class::*)(Parameter...)const> {
  using result_type = Return;
  using class_type = Class;
  static constexpr bool is_method = true;
  static constexpr bool is_noexcept = false;
  static constexpr bool is_const_method = true;
  static constexpr size_t parameter_count = sizeof...(Parameter);
  template<size_t N>
  using nth_arg_type = std::tuple_element_t<N, std::tuple<Parameter...>>;
};

struct function_score {
  size_t index;
  int value;
};

template<class Parameter, class Argument>
constexpr int compute_score() noexcept {
  if( std::is_same_v<std::remove_cv_t<Argument>,std::remove_cv_t<Parameter>> ) return 2;
  if( std::is_base_of_v<Parameter, Argument> ) return 1;
  return 0;
}

template<class ClassA, class ClassB, auto ... Entries>
struct function_scores {
  constexpr function_scores() noexcept :
    function_scores<ClassA,ClassB, Entries...>{std::make_index_sequence<sizeof...(Entries)>{}} {}
  constexpr int highest() const noexcept {
    int maxval = 0;
    int index = -1;
    //std::max_element did not compile here
    for(size_t i = 0; i < scores.size(); i++) {
      if (scores[i].value > maxval) {
        maxval = scores[i].value;
        index = i;
      }
    }
    return index;
  }
  // Provisioning only, no implementation provided
  template<class Stream>
  void print_diagonstics(Stream& out) const;
private:
  using base = std::array<function_score,sizeof...(Entries)>;
  using argumentA = std::remove_reference_t<ClassA>;
  using argumentB = std::remove_reference_t<ClassB>;
  std::array<function_score,sizeof...(Entries)> scores;
  template<auto Entry>
  static constexpr function_score make_score(size_t index) noexcept {
    using entry = function_traits<decltype(Entry)>;
    if constexpr(entry::is_method) {
      using parameterA = std::remove_reference_t<typename entry::class_type>;
      using parameterB = std::remove_reference_t<typename entry::template nth_arg_type<0>>;
      return function_score { index, compute_score<parameterA,argumentA>() * compute_score<parameterB,argumentB>() };
    } else {
      using parameterA = std::remove_reference_t<typename entry::template nth_arg_type<0>>;
      using parameterB = std::remove_reference_t<typename entry::template nth_arg_type<1>>;
      return function_score { index, compute_score<parameterA,argumentA>() * compute_score<parameterB,argumentB>() };
    }
  }
  template<size_t ... I>
  constexpr function_scores(std::index_sequence<I...>) noexcept : scores { make_score<Entries>(I) ... } {}
};

template<class ClassA, class ClassB, auto ... Entries>
constexpr int find_best_match() noexcept {
  constexpr auto sc = function_scores<ClassA, ClassB, Entries...>{};
  return sc.highest();
}

template<template<size_t> class Template, size_t N>
class jumpvector : public std::array<typename Template<0>::value_type,N> {
public:
  using value_type = typename Template<0>::value_type;
  constexpr jumpvector() : jumpvector<Template,N>(std::make_index_sequence<N>()) {}
private:
  template<size_t ... I>
  constexpr jumpvector(std::index_sequence<I...>) : std::array<value_type,N> { &Template<I>::function ... } {}
};

template<template<size_t, size_t> class Template, size_t N>
struct reduce {
  template<size_t M>
  using type = Template<N,M>;
};

template<template<size_t, size_t> class Template, size_t N, size_t M>
class jumpmatrix : public std::array<std::array<typename Template<0,0>::value_type, M>, N> {
public:
  using value_type = std::array<typename Template<0,0>::value_type, M>;
  constexpr jumpmatrix() : jumpmatrix<Template, N, M>(std::make_index_sequence<N>()) {}
private:
  template<size_t ... I>
  constexpr jumpmatrix(std::index_sequence<I...>) : std::array<value_type, N> { jumpvector<reduce<Template,I>::template type,M>{} ... } {}
};


template<size_t N, auto Entry, auto ... Entries>
constexpr auto get_entry() {
  if constexpr(N == 0) return Entry;
  else return get_entry<N-1, Entries...>();
}
} //namespace details

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"

/*
   multimethod - dispatches calls to the first suitable function or a method from the list
   Usage:
   multimethod<ReturnType,
    &function1,
    &function2,
    &MyClass::method>::dispatch(a,b);
   Requirements to the parameter classes:
     - virtual table,  e.g. hierarchy defines at least one virtual method
     - either enabled RTTI
     - or hierarchy defines template instnace_of<Class>
*/

template<typename ReturnType, auto ... Entries>
struct multimethod {
  template<class ... Arguments>
  static auto dispatch(Arguments& ... arguments) {
    ReturnType value;
    // GCC wrongly complains suggest parentheses around '&&' within '||'
    if (((details::entry<Entries>::matches(arguments...)
      and ((value = details::entry<Entries>::call(arguments...)),true)) or ...))
      return value;
    else
      LOGIC_ERROR("Dispatcher failure");
  }
  template<class Target, class ... Arguments>
  static auto call(Target& target,Arguments& ... arguments) {
    ReturnType value;
    if (((details::entry<Entries>::matches(target, arguments...)
      and ((value = details::entry<Entries>::call(target, arguments...)),true)) or ...))
      return value;
    else
      LOGIC_ERROR("Dispatcher failure");
  }
  template<class Target, class ... Arguments>
  static auto call(const Target& target,Arguments& ... arguments) {
    ReturnType value;
    if (((details::entry<Entries>::matches(target, arguments...)
      and ((value = details::entry<Entries>::call(target, arguments...)),true)) or ...))
      return value;
    else
      LOGIC_ERROR("Dispatcher failure");
  }
};
#pragma GCC diagnostic pop

/*
   matrixdispatch - dispatches calls to the best suitable function or a method from the list
   Usage:
   matrixdispatch<FunctionType,
    &function1,
    &function2,
    &MyClass::method>::dispatch(a,b);
   Requirements to the parameter classes:
     - virtual method size_t classID()
     - sequentially numbered classes
     - backward mapping ID-to-class via DomainA, DomainB classes
*/

template<typename FunctionType, class DomainA, class DomainB, auto ... Entries>
class matrixdispatch {
public:
  using entry = details::function_traits<FunctionType>;
  using result_type = typename entry::result_type;
  using parama_type = typename entry::template nth_arg_type<0>;
  using paramb_type = typename entry::template nth_arg_type<1>;
  using domainA = DomainA;
  using domainB = DomainB;
  constexpr matrixdispatch() noexcept {}
private:
  static_assert(entry::parameter_count==2,"FunctionType required to have two parameters");
  static constexpr bool no_except = entry::is_noexcept;
  template<size_t A, size_t B>
  struct func {
    static result_type function(parama_type& a, paramb_type& b) noexcept(no_except) {
      using typeA = typename DomainA::template type<A+first_concreteA>;
      using typeB = typename DomainB::template type<B+first_concreteB>;
      constexpr auto best_match = details::find_best_match<typeA, typeB, Entries...>();
      if constexpr(best_match >= 0) {
          using selected = std::tuple_element_t<best_match, std::tuple<details::function_traits<decltype(Entries)>...>>;
          if constexpr(selected::is_method) {
            constexpr auto f = details::get_entry<best_match, Entries...>();
            return (details::down_cast<typeA>(a).*f)(details::down_cast<typeB>(b));
          } else {
            constexpr auto f = details::get_entry<best_match, Entries...>();
            return (*f)(details::down_cast<typeA>(a), details::down_cast<typeB>(b));
          }
      } else {
        print_diagonstics<A,B>(a.classID(), b.classID());
        LOGIC_ERROR("Dispatcher failure");
      }
    }
    using value_type = decltype(&function);
  };
  static constexpr size_t first_concreteA = details::get_first_concrete<DomainA>(0);
  static constexpr size_t first_concreteB = details::get_first_concrete<DomainB>(0);
  static constexpr size_t sizeA = DomainA::size - first_concreteA;
  static constexpr size_t sizeB = DomainB::size - first_concreteB;
  static constexpr details::jumpmatrix<func, sizeA, sizeB> matrix {};
  template<size_t A, size_t B>
  using scores_type = details::function_scores<typename DomainA::template type<A>,
                                           typename DomainB::template type<B>, Entries...>;
public:
  // Need for this function indicates possibility of dispatch failure
  // The users are encouraged to provide their own ad-hoc implementation for the diagnostics purposes
  template<size_t A, size_t B>
  static void print_diagonstics(size_t idA, size_t idB);
  // matrix element function
  static result_type dispatch(parama_type arg1, paramb_type arg2) noexcept(no_except) {
      const size_t idA = arg1.classID();
      const size_t idB = arg2.classID();
      expects(idA >= first_concreteA and idA < DomainA::size, "arg1.classID() out of boundaries");
      expects(idB >= first_concreteB and idB < DomainB::size, "arg2.classID() out of boundaries");
      return matrix[idA  - first_concreteA][idB - first_concreteB](arg1, arg2);
  }
};
}
