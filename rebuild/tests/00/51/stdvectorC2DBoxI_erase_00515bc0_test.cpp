#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
#include <cstdio>
#include <stddef.h>
class C2DBoxI {                                            // Ego_r.pdb, 16 bytes
public:
    long TLX;                                             // +0x00
    long TLY;                                             // +0x04
    long BRX;                                             // +0x08
    long BRY;                                             // +0x0C
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                 _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
__forceinline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}

typedef C2DBoxI T;
int main()
{
    T v[6];
    for (int i = 0; i < 6; ++i) {
        v[i].TLX = (long)(i * 10 + 0);
        v[i].TLY = (long)(i * 10 + 1);
        v[i].BRX = (long)(i * 10 + 2);
        v[i].BRY = (long)(i * 10 + 3);
    }
    std::vector<T> vec;
    vec._M_start = v; vec._M_finish = v + 6; vec._M_end_of_storage._M_data = v + 6;
    T* r = vec.erase(v + 1, v + 3);                 // remove elements 1 and 2
    if (r != v + 1 || vec._M_finish != v + 4) { std::printf("00515bc0_TEST FAIL\n"); return 1; }
    static const int expect[4] = { 0, 3, 4, 5 };
    for (int i = 0; i < 4; ++i) {
        int src = expect[i];
        if (v[i].TLX != (long)(src * 10 + 0) || v[i].TLY != (long)(src * 10 + 1) || v[i].BRX != (long)(src * 10 + 2) || v[i].BRY != (long)(src * 10 + 3)) { std::printf("00515bc0_TEST FAIL\n"); return 1; }
    }
    r = vec.erase(v + 4, v + 4);                    // empty range at the end
    if (r != v + 4 || vec._M_finish != v + 4) { std::printf("00515bc0_TEST FAIL\n"); return 1; }
    r = vec.erase(v, v + 4);                        // clear
    if (r != v || vec._M_finish != v) { std::printf("00515bc0_TEST FAIL\n"); return 1; }
    std::printf("00515bc0_TEST PASS\n");
    return 0;
}