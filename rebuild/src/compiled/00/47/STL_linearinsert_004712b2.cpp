// _STL::__linear_insert(pair<unsigned long,CDefString>* first, pair<unsigned long,CDefString>* last, pair<unsigned long,CDefString> val, CKeyPairCompareLess<unsigned long,CDefString> comp) -- one insertion-sort step of the sound map's criteria entries: a key below the first entry shifts the whole run up one slot (copy_backward) and lands at the front, otherwise it walks down from `last` (the inlined unguarded linear insert)
// Instantiation: _STL::__linear_insert<pair<unsigned long,CDefString>*,pair<unsigned long,CDefString>,CKeyPairCompareLess<unsigned long,CDefString> >
// Leaf: ??$__linear_insert@PAU?$pair@KVCDefString@@@_STL@@U12@V?$CKeyPairCompareLess@KVCDefString@@@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004712B2, 92 bytes (size-optimised def TU, no padding), __fastcall
//   (first ecx, last edx; the 8-byte value and the 4-byte comparator slot on the stack), `ret 0Ch`.
// Identity: the only call site is 0x004712A0 (the 8-byte __insertion_sort just above it) in CSoundMap::CriteriaMap's
//   CVectorMap<unsigned long, CDefString, ..>::Sort chain (CVectorMap::Sort @ 0x0047119A); the 12-byte CSoundPair twin is 0x00470E08 (same source). The catalog name `std::sort_insertion_8byte`
//   is descriptive.
// Callees: __copy_backward_ptrs(first, last, result, const __false_type&) @ 0x0047130E (landed, out of line).
// Shape: STLport 4.6 `__linear_insert` with `copy_backward` inlined: its `_IsOKToMemCpy(..)._Answer()` tag is the zeroed
//   static `_Answer()` value-initialising the empty POD `__false_type` (`xor eax,eax ; stosb`, the returned byte copied
//   back onto itself; a user constructor on __false_type drops both and is 16 bytes short), its address pushed with
//   `last + 1`; `*first = val` is two dword stores (the key still in esi). The else arm is the inlined
//   `__unguarded_linear_insert` with its by-value `val` held in esi/edi.
//   Frame pointer: `optimize("y", off)` + `optimize("s", on)` as the module's other landed bodies.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const _STL::pair<Key, Value>& a, const _STL::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace _STL {
struct __false_type { };

template <class _Tp1, class _Tp2> struct _OKToMemCpy {
    typedef __false_type _Type;
    static _Type _Answer() { return _Type(); }
};
template <class _Tp1, class _Tp2>
__forceinline _OKToMemCpy<_Tp1, _Tp2> _IsOKToMemCpy(_Tp1*, _Tp2*) { return _OKToMemCpy<_Tp1, _Tp2>(); }

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result, const __false_type&);   // 0x0047130E

template <class _Tp1, class _Tp2>
__forceinline _Tp2* copy_backward(_Tp1* __first, _Tp1* __last, _Tp2* __result)
{
    return __copy_backward_ptrs(__first, __last, __result, _IsOKToMemCpy(__first, __result)._Answer());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

typedef pair<unsigned long, CDefString> _CriteriaEntry;
template void __fastcall __linear_insert(_CriteriaEntry*, _CriteriaEntry*, _CriteriaEntry, CKeyPairCompareLess<unsigned long, CDefString>);
}