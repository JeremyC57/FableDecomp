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
// Model of _STL::vector<pair<unsigned long,CDefClassBase::CSubDefInfo> >::vector(const vector&) @ 0x0046EECF:
// allocate other.size() entries and copy every (key, sub-def info) pair.

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
    _Vector_base(unsigned int __n, const _Alloc& __a)   // test copy of 0x0046EF33
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }      // 0x0046EF33
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef vector<_Tp, _Alloc> _Self;
    typedef _Alloc allocator_type;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return (const allocator_type&)this->_M_end_of_storage; }
    vector(const _Self& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __false_type());
    }
};

typedef pair<unsigned long, CDefClassBase::CSubDefInfo> _SubDefEntry;
}
int main()
{
    bool ok = true;
    typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> E;
    typedef _STL::vector<E> V;
    E* items = (E*)malloc(3 * sizeof(E));
    for (int i = 0; i < 3; ++i) {
        items[i].first = 10 + i;
        items[i].second.DefIndex = 100 + i;
        items[i].second.OriginalParentDefIndex = 200 + i;
    }
    union { double align; unsigned char bytes[sizeof(V)]; } raw, rawEmpty;
    V* from = (V*)raw.bytes;
    from->_M_start = items;
    from->_M_finish = items + 3;
    from->_M_end_of_storage._M_data = items + 3;
    V copy(*from);
    if (copy._M_start == items || copy._M_finish - copy._M_start != 3 || copy._M_end_of_storage._M_data != copy._M_start + 3)
        ok = false;
    for (int i = 0; i < 3; ++i)
        if (copy._M_start[i].first != (unsigned long)(10 + i) || copy._M_start[i].second.DefIndex != 100 + i ||
            copy._M_start[i].second.OriginalParentDefIndex != 200 + i)
            ok = false;
    V* none = (V*)rawEmpty.bytes;
    none->_M_start = 0;
    none->_M_finish = 0;
    none->_M_end_of_storage._M_data = 0;
    V empty(*none);
    if (empty._M_start != 0 || empty._M_finish != 0)
        ok = false;
    if (!ok) { printf("0046eecf_TEST FAIL\n"); return 1; }
    printf("0046eecf_TEST PASS\n");
    return 0;
}