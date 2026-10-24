// _STL::__unguarded_insertion_sort_aux(pair<long, CLongMapValue8>* first, pair<long, CLongMapValue8>* last, pair<long, CLongMapValue8>*, CKeyPairCompareLess comp) -- STLport insertion-sort pass with no lower-bound test over a long-keyed vector map of 12-byte entries, each entry sifted down by its signed key
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@JVCLongMapValue8@@
// Retail 0x0086F3E0, 0x67 bytes (+ int3 padding to 0x70), `ret 8` (/Gr: first in ecx, last in edx, the value-type tag
// pointer and the empty comparator on the stack). No callees.
// Callers: 0x0086FE95 (__final_insertion_sort 0x0086FE60: insertion sort over the first 16 entries, then this over
// [first + 16, last)) and 0x0086F8A7 (__unguarded_insertion_sort); the sort (0x008701D0) is reached from the map's
// Insert (0x0086F600) and its mem-dump reader 0x0086F680 (called from CCreatureDef::Transfer-family code at 0x00675E93).
// Element: pair<long, 8-byte POD> (0x0C bytes). The value type is NOT identified: the FableWin/Ego_r name sets hold
// several long-keyed maps of 8-byte values (CTCHeroMorph::CTextureLayer, CMusicManager::CRegisteredMusicEntry), none tied
// to this code by a second source, so CLongMapValue8 is a descriptive name. Byte twin of 0x0086AEA0 (the unsigned-key
// CRandomAppearanceMorph::CTextureMorph map) with the signed compare (`jge`/`jl`).
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)` with the linear insert
// inlined: the by-value T(*i) is three registers/frame slots (key in edi), `*last = *next` and `*last = val` the
// implicit assignment.
// Label: the manifest's "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is a propagated label.
class CLongMapValue8                                 // 8 bytes of POD (descriptive)
{
public:
    unsigned long Words[2];
};

namespace _STL {
template <class _T1, class _T2> struct pair
{
    _T1 first;                                       // +0x00
    _T2 second;                                      // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

template <class K, class V> class CKeyPairCompareLess
{
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<long, CLongMapValue8> TLongMapPair;

template void __fastcall _STL::__unguarded_insertion_sort_aux<TLongMapPair*, TLongMapPair,
    CKeyPairCompareLess<long, CLongMapValue8> >(TLongMapPair*, TLongMapPair*,
    TLongMapPair*, CKeyPairCompareLess<long, CLongMapValue8>);