#pragma once

namespace multimethods {
/*
  resolve - a helper template for resolving overloaded functions or methods
  Usage:
    resolve<Shape*, const Circle&>{}(&Rect::intersect)
*/
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
