#pragma once
#include <type_traits>
#if __cpp_exceptions >=  199711
#	include <stdexcept>
#	define LOGIC_ERROR(msg) throw std::logic_error(msg)
#else
#   include <cassert>
#	define LOGIC_ERROR(msg) assert(!msg)
#endif

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
    return (*Function)((Parameter)(argument)...);
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
    return ((Target&)(target).*Method)((Parameter&)(argument)...);
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
    return ((const Target&)(target).*Method)((Parameter&)(argument)...);
  }
};
} //namespace details
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
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
}
