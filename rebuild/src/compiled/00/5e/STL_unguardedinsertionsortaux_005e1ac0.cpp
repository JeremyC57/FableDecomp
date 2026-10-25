// std::__unguarded_insertion_sort_aux(pair<unsigned long,CAnimationEntry>* first, pair<unsigned long,CAnimationEntry>* last, pair<unsigned long,CAnimationEntry>*, CKeyPairCompareLess<unsigned long,CAnimationEntry> comp) -- STLport final unguarded insertion pass over the CRC-keyed animation entries
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E1AC0, 81 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx, last in edx
//   -> ebx; the value-type tag pointer and the comparator word on the stack, `ret 8`).
// Identity: STLport 4.6 `__unguarded_insertion_sort_aux` of the CVectorMap<unsigned long, CAnimationEntry>
//   (CAnimationSet::Anims) sort of the animation TU: the callee is the landed __unguarded_linear_insert 0x005E10B0 over
//   the 0x14-byte pair<unsigned long, CAnimationEntry> (Ego_r.pdb CAnimationEntry: BankIndex +0, CDefString AnimName +4,
//   GroupName +8, CIVCountedPointer<CAnimComponentMap> Components +0xc). Callers: 0x005E2167, 0x005E34E6. The catalog's
//   "vector<CGraphicInfo>::_Destroy" is propagated.
// Callee: __unguarded_linear_insert 0x005E10B0 (out of line; it owns and releases its by-value val).
// Shape: `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the pair's
//   user copy constructor builds the 20-byte argument in its stack slot (four words, then the counted pointer's copy:
//   null test + `inc [edx+4]`); the comparator stays declaration-only.
struct CAnimComponentMap {                              // intrusively counted: vptr, count at +4
    virtual void v00();
    virtual void Delete();                              // slot 1 (+0x04)
    long IVRefCount;                                    // +0x04
};

template <class T> struct CIVCountedPointer {           // one word: the intrusively counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

struct CDefString {                                     // one word: the def string table position
    long TablePos;                                      // +0x00
};

struct CAnimationEntry {                                // Ego_r.pdb, 0x10 bytes
    long                                 BankIndex;     // +0x00
    CDefString                           AnimName;      // +0x04
    CDefString                           GroupName;     // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;    // +0x0c
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x005E10B0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value (one stack word)
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

typedef std::pair<unsigned long, CAnimationEntry> TAnimationEntryPair;
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> TAnimationEntryLess;
template void __fastcall std::__unguarded_insertion_sort_aux<TAnimationEntryPair*, TAnimationEntryPair, TAnimationEntryLess>(
    TAnimationEntryPair*, TAnimationEntryPair*, TAnimationEntryPair*, TAnimationEntryLess);