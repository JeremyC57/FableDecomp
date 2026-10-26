// std::vector::reserve(unsigned int n) -- (vector<_List_iterator<CAnimSet, _Const_traits<CAnimSet> > >) STLport reserve: when the capacity is below n, reallocate to n elements through the out-of-line _M_allocate_and_copy, free the old block and keep the size
// Leaf: ?reserve@?$vector@U?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@std@@V?$allocator@U?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@std@@@2@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00710B00, 114 bytes (+ int3 padding to 128), thiscall, `ret 4`.
// Identity (proven): std::vector<_List_iterator<CAnimSet, _Const_traits<CAnimSet> > >::reserve(unsigned int); evidence: its only caller is CAnimateThing::CalcInterpolatedMeshAnim 0x0070D940 (0x0070DF3E) on the function-static vector at 0x013BAC20 (`mov ecx, 0x13bac20`) -- Ego_r.pdb `CAnimateThing::CalcInterpolatedMeshAnim'::`dynamic atexit destructor for 'sorted_combination_anims'', vector<list<CAnimSet>::const_iterator> in both PDBs (the STLport const iterator _List_iterator<CAnimSet, _Const_traits<CAnimSet> >, 4 bytes; J004 0x00711080 is its _M_insert_overflow); FableWin.pdb emits its reserve.
// Label: the manifest's "CAnimSet::Reserve_Reallocate" is propagated (family J009: six reserve bodies over 4-byte elements with a trivial
//   destructor; retail has no identical-code folding, so each address is one element type).
// Callees: _M_allocate_and_copy<_List_iterator<CAnimSet, _Const_traits<CAnimSet> >*>(n, first, last) 0x0070FB40 (out of line), free 0x00BFEA14 (the inline
//   _M_clear: the allocator's null-tested free, no destroy loop), malloc 0x00BFEA0E (the inline allocate on the
//   empty-vector path).
// Shape: STLport 4.x verbatim: `if (capacity() < n) { old_size = size(); if (_M_start) { tmp = _M_allocate_and_copy(n,
//   _M_start, _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old_size, tmp + n); }`.
// Model: the landed 0x004CC9B0 (vector<pair<ETCInterfaceType, CTCBase*> >).
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (MSVCR71 import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (MSVCR71 import thunk)

class CAnimSet;
namespace std {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _List_iterator_base { _List_node_base* _M_node; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base { };   // list<CAnimSet>::const_iterator
}

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
    _Tp* _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x0070FB40
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

template void vector<_List_iterator<CAnimSet, _Const_traits<CAnimSet> > >::reserve(unsigned int);
}