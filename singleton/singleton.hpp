#pragma once

#include <type_traits>

#include <singleton/non_copyable.hpp>
#include <singleton/non_movable.hpp>

namespace patterns {

/**
 * @brief Inherit this class to make a child class a singleton
 *
 *
 * ATTENTION!
 * You MUST have only one private constructor without arguments!
 * You should not have any public constructors to prevent your singleton-class
 * instantiation not throw the `instance` interface.
 *
 * You should make this class a friend of yours to make it possible to
 * instantiate your class inside the `instance` method.
 *
 * If you need to pass arguments to singleton constructor from outside make your
 * own class. May be it is better to use a fabric pattern in this case.
 *
 * For example:
 *
 * @code{.cpp}
 * class A : public Singleton<A> {
 *     friend class Singleton<A>; // we need it to call A constructor
 *                                // from Singleton<A> constructor.
 *
 *     private:
 *        A() {} // nobody should allowed to use private A constructor
 *               // but Singleton<A> because it is a friend of A
 *               // For over guys from outside Singleton<A> provides a
 *               // public static function instance().
 * }
 * @endcode
 *
 */
template<typename T>
struct Singleton : NonMovable<Singleton<T>>, NonCopyable<Singleton<T>>
{

    static T& instance()
    {
        static T instance;
        instance_ptr = &instance;

        return instance;
    }

 protected:
    Singleton()
    {
        static_assert(
          !std::is_default_constructible_v<T>,
          "Your singleton class must not has any public constructors!");
    }

    static T* instance_ptr;
};

template<typename T>
T* Singleton<T>::instance_ptr = nullptr;

}  // namespace patterns
