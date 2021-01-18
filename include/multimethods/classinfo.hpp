#pragma once
#include <fnv1c/fnv1c.hpp>
namespace multimethods {
template<class Class>
constexpr auto class_hash() noexcept {
  return fnv1c::hash<>(std::string_view(__PRETTY_FUNCTION__));
}

#if __cpp_rtti >= 199711
using class_info = std::type_info;
template<class Class>
const class_info& classinfo() noexcept {
  return typeid(Class);
}
template<class Class>
const class_info& classinfo(const Class&) noexcept {
  return typeid(Class);
}
#else
using class_info = size_t;
template<class Class>
class_info classinfo() noexcept {
  return Class::classid;
}
template<class Class>
class_info classinfo(const Class&) noexcept {
  return Class::classid;
}
#endif
}
