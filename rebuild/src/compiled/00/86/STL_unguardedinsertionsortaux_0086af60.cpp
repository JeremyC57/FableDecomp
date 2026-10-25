// std::__unguarded_insertion_sort_aux(pair<long, CRandomAppearanceMorph::CSkeletalMorph>* first, pair<long, CRandomAppearanceMorph::CSkeletalMorph>* last, pair<long, CRandomAppearanceMorph::CSkeletalMorph>*, CKeyPairCompareLess comp) -- STLport insertion-sort pass with no lower-bound test over the skeletal-morph vector map, one out-of-line linear insert per entry
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@JVCSkeletalMorph@CRandomAppearanceMorph@@
// Retail 0x0086AF60, 0x40 bytes (+ int3 padding to 0x50), `ret 8` (/Gr: first in ecx, last in edx, the value-type tag
// pointer and the empty comparator on the stack).
// Callers: 0x0086B447 (__unguarded_insertion_sort), 0x0086D06C (__final_insertion_sort, [first + 16, last)) and 0x0086DDD9.
// Callee: __unguarded_linear_insert<pair<long, CSkeletalMorph>*, pair<...>, CKeyPairCompareLess<...> > @ 0x0086A6E0 (NOT
// inlined for the 16-byte element: last in ecx, the 16-byte value and the comparator on the stack, `ret 14h`; its body
// is the signed `jge`/`jl` sift).
// Element: pair<long, CRandomAppearanceMorph::CSkeletalMorph> (0x10 bytes; Ego_r CRandomAppearanceMorph::SkeletalMorphs is
// CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> >; the map's Count 0x0086A9D0 divides by 16).
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)`: the by-value T(*i) is built
// in the outgoing argument block by the pair's user copy constructor (key dword, then the 12-byte POD second struct-copied
// through `lea edx,[esi+4]`), after the comparator push.
// Label: the manifest's "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is a propagated label.
class CRandomAppearanceMorph
{
public:
    class CSkeletalMorph                             // 12 bytes of POD
    {
    public:
        long          GroupID;                       // +0x00
        unsigned long Words[2];                      // +0x04
    };
};

namespace std {
template <class _T1, class _T2> struct pair
{
    _T1 first;                                       // +0x00
    _T2 second;                                      // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphPair;

template void __fastcall std::__unguarded_insertion_sort_aux<TSkeletalMorphPair*, TSkeletalMorphPair,
    CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> >(TSkeletalMorphPair*, TSkeletalMorphPair*,
    TSkeletalMorphPair*, CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph>);