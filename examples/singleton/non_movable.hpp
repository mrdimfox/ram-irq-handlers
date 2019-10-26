#pragma once

namespace patterns {

template<class T>
class NonMovable
{
 protected:
    NonMovable() {}
    ~NonMovable() {}
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;
};

}  // namespace patterns


