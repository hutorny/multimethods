#pragma once
#if __cpp_exceptions >=  199711
#   include <stdexcept>
#   define LOGIC_ERROR(msg) throw std::logic_error(msg)
    inline void expects(bool condition, [[maybe_unused]] const char* msg) {
      if(not condition) throw std::logic_error(msg);
    }
#else
#   include <cassert>
#   include <cstdlib>
#   define LOGIC_ERROR(msg) do { assert(!msg); abort(); } while(1)
    inline void expects(bool condition, [[maybe_unused]] const char* msg) {
      if(not condition) assert(!msg);
    }
#endif
