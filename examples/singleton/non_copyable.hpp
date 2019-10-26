#pragma once

namespace patterns {

template<class T>
class NonCopyable
{
 protected:
    NonCopyable() {}
    ~NonCopyable() {}
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

}  // namespace patterns


