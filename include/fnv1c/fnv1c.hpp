#pragma once
#include <string_view>

namespace fnv1c {
/* FNV-1b hash function with extra rotation
 * https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
 */
template<typename T = std::size_t>
inline T hash(const char* str, std::size_t size) noexcept;

template<typename T>
inline constexpr T ror(T x, unsigned N) noexcept {
  return (x << (sizeof(T)*8 - N)) | ((x) >> (N));
}

template<>
inline constexpr uint32_t hash<uint32_t>(const char* str, std::size_t size) noexcept {
  constexpr uint32_t val32   = 0x811c9dc5;
  constexpr uint32_t prime32 = 16777619;

  uint32_t result = val32;
  while(size--) {
    result ^= uint32_t(*str);
    result  = ror(result, (8+(0xF&(*str++)))); /* added rotation 8-23 bits */
    result *= prime32;
    }
    return result;
}

template<>
inline constexpr uint64_t hash<uint64_t>(const char* str, std::size_t size) noexcept {
  constexpr uint64_t val64   = 0xcbf29ce484222325;
  constexpr uint64_t prime64 = 1099511628211ULL;
  uint64_t result = val64;
  while(size--) {
    result ^= uint64_t(*str);
    result  = ror(result, (8+(*str++)%48)); /* added rotation 8-55 bits */
    result *= prime64;
  }
  return result;
}

template<typename T = std::size_t>
inline constexpr T hash(const std::string_view str) noexcept {
  return hash<T>(str.data(), str.size());
}

} // namespace fnv1c
