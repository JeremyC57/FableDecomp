// std::vector::reserve(unsigned int n) -- (vector<ESurfaceMultisampleType >) STLport reserve: when the capacity is below n, reallocate to n elements through the out-of-line _M_allocate_and_copy, free the old block and keep the size
// Leaf: ?reserve@?$vector@W4ESurfaceMultisampleType@@V?$allocator@W4ESurfaceMultisampleType@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C2780, 114 bytes (+ int3 padding to 128), thiscall, `ret 4`.
// Identity (proven): std::vector<ESurfaceMultisampleType >::reserve(unsigned int); evidence: its only caller is 0x009C05C0 (0x009C06F8) -- CDisplayManager::EnumerateAntiAliasingModes(CArray<ESurfaceMultisampleType>&) (Ego_r.pdb ?EnumerateAntiAliasingModes@CDisplayManager@@QAEXAAV?$CArray@W4ESurfaceMultisampleType@@@@@Z): it walks BackBufferFormats (+0x298) x MultisampleTypes (+0x2A4) collecting the accepted types in a local set, clears the output array and reserves it to the set's size; FableWin.pdb emits vector<ESurfaceMultisampleType>::reserve.
// Label: the manifest's "std::vector_reserve_int" is propagated (family J009: six reserve bodies over 4-byte elements with a trivial
//   destructor; retail has no identical-code folding, so each address is one element type).
// Callees: _M_allocate_and_copy<ESurfaceMultisampleType*>(n, first, last) 0x009C1970 (out of line), free 0x00BFEA14 (the inline
//   _M_clear: the allocator's null-tested free, no destroy loop), malloc 0x00BFEA0E (the inline allocate on the
//   empty-vector path).
// Shape: STLport 4.x verbatim: `if (capacity() < n) { old_size = size(); if (_M_start) { tmp = _M_allocate_and_copy(n,
//   _M_start, _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old_size, tmp + n); }`.
// Model: the landed 0x004CC9B0 (vector<pair<ETCInterfaceType, CTCBase*> >).
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (MSVCR71 import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (MSVCR71 import thunk)

enum ESurfaceMultisampleType { ESurfaceMultisampleType_Force32 = 0x7fffffff };   // enumerators not needed

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {          // STLport pair: explicit copy constructor next to the converting one
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {   // the end-of-storage pointer over the empty allocator
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport vector (12 bytes)
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage._M_data - _M_start); }
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x009C1970
    void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage._M_data = __e; }
    void reserve(size_type __n)
    {
        if (capacity() < __n) {
            const size_type __old_size = size();
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
};

template void vector<ESurfaceMultisampleType >::reserve(unsigned int);
}