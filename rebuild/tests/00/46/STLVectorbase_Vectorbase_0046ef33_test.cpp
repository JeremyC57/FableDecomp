#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Model of _STL::_Vector_base<pair<unsigned long,CDefClassBase::CSubDefInfo> >::_Vector_base(n, a) @ 0x0046EF33:
// allocate room for n entries (none for n == 0), empty, with the end of storage n entries on.

class CDefClassBase {
public:
    class CSubDefInfo {                                     // Ego_r.pdb, 8 bytes
    public:
        long DefIndex;                                      // +0x0
        long OriginalParentDefIndex;                        // +0x4
    };
};

namespace _STL {
struct __false_type { };

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

typedef pair<unsigned long, CDefClassBase::CSubDefInfo> _SubDefEntry;
}
int main()
{
    bool ok = true;
    typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> E;
    _STL::_Vector_base<E, _STL::allocator<E> > b(3, _STL::allocator<E>());
    if (b._M_start == 0 || b._M_finish != b._M_start || b._M_end_of_storage._M_data != b._M_start + 3)
        ok = false;
    _STL::_Vector_base<E, _STL::allocator<E> > z(0, _STL::allocator<E>());
    if (z._M_start != 0 || z._M_finish != 0 || z._M_end_of_storage._M_data != 0)
        ok = false;
    if (!ok) { printf("0046ef33_TEST FAIL\n"); return 1; }
    printf("0046ef33_TEST PASS\n");
    return 0;
}