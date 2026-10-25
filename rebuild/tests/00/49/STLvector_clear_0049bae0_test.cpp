// Behaviour test for _STL::vector<float>::clear (0x0049BAE0): finish drops back to start, storage and bytes untouched.
#include <cstdio>
#include <string.h>

namespace _STL {
struct __true_type { __true_type() {} };
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
            + ((const char*)__last - (const char*)__first);
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) {}
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

int main()
{
    float buffer[8] = { 1, 2, 3, 4, 5, 0, 0, 0 };
    _STL::vector<float> v;
    v._M_start = buffer; v._M_finish = buffer + 5; v._M_end_of_storage = buffer + 8;
    v.clear();
    if (v._M_start != buffer || v._M_finish != buffer || v._M_end_of_storage != buffer + 8 || buffer[4] != 5.0f)
        { std::printf("0049bae0_TEST FAIL\n"); return 1; }
    v.clear();   // already empty
    if (v._M_finish != buffer) { std::printf("0049bae0_TEST FAIL\n"); return 1; }
    std::printf("0049bae0_TEST PASS\n");
    return 0;
}