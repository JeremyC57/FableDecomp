// _STL::__linear_insert(pair<unsigned long,CSoundPair>* first, pair<unsigned long,CSoundPair>* last, pair<unsigned long,CSoundPair> val, CKeyPairCompareLess<unsigned long,CSoundPair> comp) -- one insertion-sort step of the sound map: a key below the first entry shifts the whole run up one slot (copy_backward) and lands at the front, otherwise it walks down from `last` (the inlined unguarded linear insert)
// Instantiation: _STL::__linear_insert<pair<unsigned long,CSoundPair>*,pair<unsigned long,CSoundPair>,CKeyPairCompareLess<unsigned long,CSoundPair> >
// Leaf: ??$__linear_insert@PAU?$pair@KVCSoundPair@@@_STL@@U12@V?$CKeyPairCompareLess@KVCSoundPair@@@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00470E08, 101 bytes (size-optimised def TU, no padding), __fastcall
//   (first ecx, last edx; the 12-byte value and the 4-byte comparator slot on the stack), `ret 10h`.
// Identity: the only caller is the landed __insertion_sort @ 0x00470DBC (the sound map's
//   CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<..> >::Sort chain from CVectorMap::Sort @ 0x00470CD8).
//   The catalog name `std::sort_insertion_12byte` is descriptive.
// Callees: __copy_backward_ptrs(first, last, result, const __false_type&) @ 0x00470E6D (out of line).
// Shape: STLport 4.6 `__linear_insert` with `copy_backward` inlined: its `_IsOKToMemCpy(..)._Answer()` tag is the zeroed
//   static `_Answer()` value-initialising the empty POD `__false_type` (`xor eax,eax ; stosb`, the returned byte copied
//   back onto itself; a user constructor on __false_type drops both and is 16 bytes short), its address pushed with
//   `last + 1`; `*first = val` is movsd x3 from the argument slot. The else arm is the inlined `__unguarded_linear_insert`:
//   its by-value `val` is the 12-byte frame local (pair's member-wise copy constructor), the key kept in ecx.
//   Frame pointer: `optimize("y", off)` + `optimize("s", on)` as the module's other landed bodies (0x00470DBC).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CSoundPair {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
public:
    long FirstSound;                                        // +0x0
    long LastSound;                                         // +0x4
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
                                                    _BidirectionalIter2 __result, const __false_type&);   // 0x00470E6D

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

typedef pair<unsigned long, CSoundPair> _SoundEntry;
template void __fastcall __linear_insert(_SoundEntry*, _SoundEntry*, _SoundEntry, CKeyPairCompareLess<unsigned long, CSoundPair>);
}