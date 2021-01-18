#pragma once

namespace multimethods {

template<typename Result, typename ... Args>
struct resolve {
   template<class Class>
   constexpr auto operator()(Result (Class::*m)(Args ...)) const noexcept {
     return m;
   }
   template<class Class>
   constexpr auto operator()(Result (Class::*m)(Args ...)const) const noexcept {
     return m;
   }
   constexpr auto operator()(Result (*m)(Args ...)) const noexcept {
     return m;
   }
};

}
