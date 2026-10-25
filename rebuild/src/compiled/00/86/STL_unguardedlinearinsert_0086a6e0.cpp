// _STL::__unguarded_linear_insert(pair<long, CRandomAppearanceMorph::CSkeletalMorph>* last, pair<long, CSkeletalMorph> val, CKeyPairCompareLess<long, CSkeletalMorph> comp) -- STLport insertion step over the skeletal-morph vector map: shift every larger-keyed entry up one slot and drop val into the hole
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086A6E0, length 80 bytes, `ret 14h`.
// Callers: __unguarded_insertion_sort_aux 0x0086AF60 (landed; 0x0086AF92) and the same map's __linear_insert 0x0086C620
//   (landed under the propagated label CCompressorZlib::SetupStream; 0x0086C681).
// Callees: none.
// Label: catalog "std::pair_insertion_sort_shift" is descriptive; STLport 4.x `__unguarded_linear_insert` verbatim.
// Element: pair<long, CRandomAppearanceMorph::CSkeletalMorph> (16 bytes; Ego_r.pdb CRandomAppearanceMorph::SkeletalMorphs is
//   CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> >); the key compare `a.first < b.first`
//   is signed (`jge` / `jl`). Same model as the landed 0x0086AF60.
// Shape: /Gr algorithm TU (`__fastcall` template: last in ecx, the 16-byte value and the empty comparator on the stack);
//   the value's key rides in edx, its second words are read from the argument slot for the final store.
class CDefString {                                          // Ego_r.pdb, 4 bytes
public:
    long TablePos;                                          // +0x00
};
class CRandomAppearanceMorph {
public:
    class CSkeletalMorph {                                  // Ego_r.pdb, 12 bytes (implicit, trivial copy)
    public:
        long       GroupID;                                 // +0x00
        CDefString Name;                                    // +0x04
        bool       AllowVariation;                          // +0x08
    };
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
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
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphPair;
typedef CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphLess;
template void __fastcall _STL::__unguarded_linear_insert<TSkeletalMorphPair*, TSkeletalMorphPair, TSkeletalMorphLess>(TSkeletalMorphPair*, TSkeletalMorphPair, TSkeletalMorphLess);