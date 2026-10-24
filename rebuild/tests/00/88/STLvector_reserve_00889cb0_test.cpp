// Behaviour test for 0x00889CB0 _STL::vector<CBlendedEnvironmentTheme>::reserve(n): when n exceeds the capacity, allocate
// n elements, copy-construct the old ones across, destroy and free the old block, keep the size; otherwise do nothing.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
#include <new>

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {                        // 500 bytes (retail 12-byte CArrays)
public:
    unsigned char _body[0x1F4];
    ~CBlendedEnvironmentTheme();                        // non-trivial (its CArray members); out of line
};
}

namespace _STL {
struct __false_type { __false_type() { } };

typedef NEnvironmentInternal::CBlendedEnvironmentTheme CTheme;
__declspec(noinline) CTheme* __fastcall __uninitialized_copy(CTheme* first, CTheme* last, CTheme* result, const __false_type&)   // 0x00889700
{
    for (; first != last; ++first, ++result)
        new (result) CTheme(*first);
    return result;
}
__declspec(noinline) void __fastcall __destroy_aux(CTheme* first, CTheme* last, const __false_type&)                            // 0x006B67A0
{
    for (; first != last; ++first)
        first->~CTheme();
}

template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp>
class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                                      // +0x00
    _Tp* _M_finish;                                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;             // +0x08

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage._M_data - _M_start); }

    _Tp* _M_allocate_and_copy(unsigned int __n, _Tp* __first, _Tp* __last)
    {
        _Tp* __result = _M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void reserve(unsigned int __n);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(unsigned int __n)
{
    if (capacity() < __n) {
        const unsigned int __old_size = size();
        _Tp* __tmp;
        if (_M_start) {
            __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
            _M_clear();
        } else {
            __tmp = _M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

}

static int g_dtors = 0;
NEnvironmentInternal::CBlendedEnvironmentTheme::~CBlendedEnvironmentTheme() { ++g_dtors; }

int main()
{
    typedef NEnvironmentInternal::CBlendedEnvironmentTheme T;
    _STL::vector<T> v; v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    int ok = 1;
    v.reserve(2);                                           // empty: a bare allocation
    if (v.capacity() != 2 || v.size() != 0 || !v._M_start || g_dtors != 0) ok = 0;
    for (int k = 0; k < 2; ++k) { memset(v._M_finish->_body, 0x10 + k, sizeof(v._M_finish->_body)); ++v._M_finish; }
    T* old = v._M_start;
    v.reserve(1);                                           // smaller: nothing happens
    if (v._M_start != old || v.capacity() != 2 || v.size() != 2) ok = 0;
    v.reserve(5);                                           // grow: copy 2, destroy 2
    if (v.capacity() != 5 || v.size() != 2 || g_dtors != 2) ok = 0;
    if (v._M_start[0]._body[0] != 0x10 || v._M_start[0]._body[499] != 0x10 || v._M_start[1]._body[250] != 0x11) ok = 0;
    printf(ok ? "00889cb0_TEST PASS\n" : "00889cb0_TEST FAIL\n");
    return ok ? 0 : 1;
}