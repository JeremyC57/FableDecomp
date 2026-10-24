// _STL::__unguarded_insertion_sort_aux(pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>* first, pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>* last, pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>*, CKeyPairCompareLess comp) -- STLport insertion-sort pass with no lower-bound test over the texture-morph vector map, each entry sifted down by its unsigned key
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@
// Retail 0x0086AEA0, 0x67 bytes (+ int3 padding to 0x70), `ret 8` (/Gr: first in ecx, last in edx, the value-type tag
// pointer and the empty comparator on the stack). No callees.
// Callers: 0x0086B3A7 (__unguarded_insertion_sort 0x0086B3A0, which pushes the comparator and a null tag) and 0x0086D025
// (__final_insertion_sort 0x0086CFF0: insertion sort over the first 16 entries, then this over [first + 16, last)); the
// sort itself (0x0086DD20) is called from TextureMorph_AddToList 0x0086BC10 and its siblings.
// Element: pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> (0x0C bytes; FableWin/Ego_r name the map
// CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<...> >); the key compare is unsigned
// (`jae`/`jb`); the 0x0086F3E0 twin is the signed-key (long) map.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)` with the linear insert
// inlined: the by-value T(*i) is three registers/frame slots (key in edi), `*last = *next` and `*last = val` the
// implicit assignment.
// Label: the manifest's "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is a propagated label.
class CRandomAppearanceMorph
{
public:
    class CTextureMorph                              // 8 bytes of POD
    {
    public:
        unsigned long Words[2];
    };
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

typedef _STL::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> TTextureMorphPair;

template void __fastcall _STL::__unguarded_insertion_sort_aux<TTextureMorphPair*, TTextureMorphPair,
    CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >(TTextureMorphPair*, TTextureMorphPair*,
    TTextureMorphPair*, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph>);