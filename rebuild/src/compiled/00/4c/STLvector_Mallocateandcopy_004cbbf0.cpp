// std::vector::_M_allocate_and_copy(unsigned int n, const pair<ETCInterfaceType, CTCBase*>* first, const pair<ETCInterfaceType, CTCBase*>* last) -- allocate room for n component-map entries and copy-construct [first, last) into it; returns the block
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CBBF0, 79 bytes, thiscall (`this` unused), `ret 0Ch`, the block in eax.
// Caller: vector<pair<ETCInterfaceType, CTCBase*> >::reserve 0x004CC9B0 (this chunk).
// Callee: malloc (MSVCR71 import thunk) 0x00BFEA0E.
// Identity: the element of CThing::Components (CVectorMap<ETCInterfaceType, CTCBase*>, 8 bytes): reserve is called from
//   CThing::CreateFromDef 0x004CA930 on Components. Byte twin of the landed
//   vector<pair<float, EConversationAttitude> >::_M_allocate_and_copy 0x00718FB0 (same 8-byte POD-like pair). The
//   catalog's "std::vector_Reallocate" is descriptive.
// Shape: STLport member template `_M_allocate_and_copy`: `allocate(n)` (`n != 0 ? malloc(n * 8) : 0`), then the inline
//   `__uninitialized_copy(first, last, result, __false_type())` placement loop (null-tested, strength-reduced to the
//   result - first offset in esi). STLport's pair carries an explicit copy constructor, which gives retail's
//   `mov edx,[ecx+4] ; mov [esi+ecx+4],edx` second-word copy.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (MSVCR71 import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (MSVCR71 import thunk)

enum ETCInterfaceType { TC_NONE = 0 };                  // the thing-component interface type
class CTCBase;

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
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};

template pair<ETCInterfaceType, CTCBase*>* vector<pair<ETCInterfaceType, CTCBase*> >::_M_allocate_and_copy<const pair<ETCInterfaceType, CTCBase*>*>(
    unsigned int, const pair<ETCInterfaceType, CTCBase*>*, const pair<ETCInterfaceType, CTCBase*>*);
}