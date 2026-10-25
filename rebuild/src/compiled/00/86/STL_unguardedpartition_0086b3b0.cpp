// _STL::__unguarded_partition(pair<long, CRandomAppearanceMorph::CSkeletalMorph>* first, pair<long, CSkeletalMorph>* last, pair<long, CSkeletalMorph> pivot, CKeyPairCompareLess<long, CSkeletalMorph> comp) -- STLport quicksort partition of the skeletal-morph vector map: walk both ends inwards past correctly placed keys and swap the strays, return the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086B3B0, length 136 bytes, `ret 14h`.
// Callers: the map's __introsort_loop 0x0086DB30 (this chunk; 0x0086DBBC).
// Callees: none (iter_swap and the key compare are inline).
// Label: catalog "std::introsort_4way_partition_int" is descriptive; STLport 4.x `__unguarded_partition` verbatim.
// Element: pair<long, CRandomAppearanceMorph::CSkeletalMorph> (16 bytes; Ego_r.pdb CRandomAppearanceMorph::SkeletalMorphs =
//   CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> >), the signed key compare (`jge` / `jl`).
//   Same model as the landed 0x0086AF60 / this chunk's 0x0086A6E0.
// Shape: /Gr algorithm TU (`__fastcall` template: first in ecx, last in edx, the 16-byte pivot and the empty comparator on
//   the stack); the pivot key is re-read from its slot at each outer pass; iter_swap's temporary keeps the key in ecx and
//   the 12-byte second in esi / ebx / edi with two spills into the 16-byte frame.
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

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphPair;
typedef CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> TSkeletalMorphLess;
template TSkeletalMorphPair* __fastcall _STL::__unguarded_partition<TSkeletalMorphPair*, TSkeletalMorphPair, TSkeletalMorphLess>(TSkeletalMorphPair*, TSkeletalMorphPair*, TSkeletalMorphPair, TSkeletalMorphLess);