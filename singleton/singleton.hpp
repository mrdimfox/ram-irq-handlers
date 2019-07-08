#pragma once

#include <type_traits>

#include <singleton/non_copyable.hpp>
#include <singleton/non_movable.hpp>

namespace patterns {

/**
 * @brief Inherit this class to make a child class a singleton
 *
 * If you make a private child constructor you should make this class a friend
 * of yours.
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
    Singleton() = default;
    static T* instance_ptr;
};

template<typename T>
T* Singleton<T>::instance_ptr = nullptr;

}  // namespace patterns
