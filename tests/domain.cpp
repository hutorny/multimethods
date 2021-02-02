#include <iostream>
#include <multimethods/domain.hpp>
#include <multimethods/logic_error.hpp>
#include <multimethods/multimethods.hpp>
#include <multimethods/resolve.hpp>

#define TRACE() \
  std::cout << __PRETTY_FUNCTION__ << std::endl;

namespace domaintest {

class Object;
class Actor;
class Obstacle;
class Item;
class Mob;
class Wall;
class Pit;
class Dog;
class Cat;

using Domain = multimethods::domain<
  Object,
  Obstacle,
  Mob,
  Actor,
  Item,
  Wall,
  Pit,
  Dog,
  Cat
>;

class Object {
public:
  static constexpr auto classid = Domain::id_of<Object>();
  virtual ~Object() {}
  virtual size_t classID() const { return classid; }
  virtual void collide(Object&) = 0;
};

class Actor : public Object {
public:
  static constexpr auto classid = Domain::id_of<Actor>();
  size_t classID() const override { return classid; }
  void collide(Object&) override {}
};

class Obstacle : public Object {
public:
  static constexpr auto classid = Domain::id_of<Obstacle>();
  size_t classID() const override { return classid; }
};

class Item : public Object {
public:
  static constexpr auto classid = Domain::id_of<Item>();
  size_t classID() const override { return classid; }
  void collide(Object&) override {}
};

class Mob : public Object {
public:
  static constexpr auto classid = Domain::id_of<Mob>();
  size_t classID() const override { return classid; }
};

class Wall : public Obstacle {
public:
  static constexpr auto classid = Domain::id_of<Wall>();
  size_t classID() const override { return classid; }
  void collide(Object&) override {};
};

class Pit : public Obstacle {
public:
  static constexpr auto classid = Domain::id_of<Pit>();
  size_t classID() const override { return classid; }
  void collide(Object&) override {};
};

class Dog : public Mob {
public:
  static constexpr auto classid = Domain::id_of<Dog>();
  size_t classID() const override { return classid; }
  void collide(Object&) override {};
};

class Cat : public Mob {
public:
  static constexpr auto classid = Domain::id_of<Cat>();
  size_t classID() const override { return classid; }
  void collide(Object&) override {};
};

template<class Class>
static Class local;

Object& instance(size_t id) {
  switch(id) {
  case Actor::classid:    return local<Actor>;
  case Item::classid:     return local<Item>;
  case Wall::classid:     return local<Wall>;
  case Pit::classid:      return local<Pit>;
  case Dog::classid:      return local<Dog>;
  case Cat::classid:      return local<Cat>;
  case Obstacle::classid:
  case Mob::classid:
    LOGIC_ERROR("Abstract class");
  default:
    LOGIC_ERROR("Out of bounds");
  }
}

void collide(Cat&, Dog&) { TRACE(); }
void collide(Cat&, Pit&) { TRACE(); }
void collide(Cat&, Wall&) { TRACE(); }
void collide(Dog&, Pit&) { TRACE(); }
void collide(Dog&, Wall&) { TRACE(); }
void collide(Mob&, Item&) { TRACE(); }
void collide(Mob&, Actor&) { TRACE(); }
void collide(Actor&, Cat&) { TRACE(); }
void collide(Actor&, Dog&) { TRACE(); }
void collide(Actor&, Mob&) { TRACE(); }
void collide(Actor&, Item&) { TRACE(); }
void collide(Actor&, Wall&) { TRACE(); }
void collide(Actor&, Pit&) { TRACE(); }

using namespace multimethods;
using dispatch = matrixdispatch<
  void (*)(Object&, Object&), Domain, Domain,
  resolve<void, Cat&, Dog&   >{}(& collide),
  resolve<void, Cat&, Pit&   >{}(& collide),
  resolve<void, Cat&, Wall&  >{}(& collide),
  resolve<void, Dog&, Pit&   >{}(& collide),
  resolve<void, Dog&, Wall&  >{}(& collide),
  resolve<void, Mob&, Item&  >{}(& collide),
  resolve<void, Mob&, Actor& >{}(& collide),
  resolve<void, Actor&, Cat& >{}(& collide),
  resolve<void, Actor&, Dog& >{}(& collide),
  resolve<void, Actor&, Mob& >{}(& collide),
  resolve<void, Actor&, Item&>{}(& collide),
  resolve<void, Actor&, Wall&>{}(& collide),
  resolve<void, Actor&, Pit& >{}(& collide),
  &Wall::collide
>;
}
namespace multimethods {
// Specializing print_diagonstics for domaintest::dispatch to
// diagnose dispatching failures at run-time
template<>
template<size_t A, size_t B>
inline void domaintest::dispatch::print_diagonstics(size_t, size_t) {}
}
namespace domaintest {
void test() {
  constexpr auto first_concrete = Domain::first_concrete();
  static_assert(Domain::validate(), "Invalid");
  std::cout << "======= domain example ======= \n";
  std::cout << "Object  : " << Object  ::classid << "\n";
  std::cout << "Actor   : " << Actor   ::classid << "\n";
  std::cout << "Obstacle: " << Obstacle::classid << "\n";
  std::cout << "Item    : " << Item    ::classid << "\n";
  std::cout << "Mob     : " << Mob     ::classid << "\n";
  std::cout << "Wall    : " << Wall    ::classid << "\n";
  std::cout << "Pit     : " << Pit     ::classid << "\n";
  std::cout << "Dog     : " << Dog     ::classid << "\n";
  std::cout << "Cat     : " << Cat     ::classid << "\n";
  std::cout << "first_concrete: " << first_concrete << "\n";
  for(size_t i = first_concrete; i < Domain::size; i++)
	  std::cout << "instance(i): " << instance(i).classID() << "\n";
  Actor actor{};
  Cat cat{};
  dispatch::dispatch(actor, cat);
}

}
