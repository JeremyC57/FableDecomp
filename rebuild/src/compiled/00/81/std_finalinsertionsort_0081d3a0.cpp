// std::__final_insertion_sort(first, last, comp) -- (pair<EHeroMorphType, CParticleMorphs::CEntry> elements, CKeyPairCompareLess<EHeroMorphType, CParticleMorphs::CEntry>) STLport sort tail: insertion-sort the first 16 elements and run the unguarded insertion over the rest, or insertion-sort a short range whole
// Leaf: ??$__final_insertion_sort@PAU?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@std@@V?$CKeyPairCompareLess@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@@@std@@YIXPAU?$pair@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@0@0V?$CKeyPairCompareLess@W4EHeroMorphType@@VCEntry@CParticleMorphs@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081D3A0, 81 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx), `ret 4`
//   (the 4-byte comparator); speed TU (`imul` reciprocal for the 40-byte count, threshold `lea edi,[ecx+280h]`).
// Identity (proven): __final_insertion_sort<pair<EHeroMorphType, CParticleMorphs::CEntry>*, CKeyPairCompareLess<EHeroMorphType, CParticleMorphs::CEntry>>; evidence: its only caller is the sort 0x0081E090 of the 40-byte (EHeroMorphType, 36-byte CEntry) map = Ego_r.pdb CParticleMorphs::Morphs, CVectorMap<EHeroMorphType, CParticleMorphs::CEntry, CKeyPairCompareLess<...> > (this chunk's H016 row 0x0081D980 resizes the same pair vector); 40-byte stride (threshold 0x280); its aux callee 0x0081A940 is the landed 40-byte CKeyPairCompareLess instance.
// Label: the manifest's "make_heap<std::pair<unsigned_long,CSoundPair>*,CKeyPairCompareLess<unsigned_long,CSoundPair>_>" is propagated across family H023 (seven bodies that differ only in the element size and the
//   call displacements; retail has no identical-code folding, so each address is one instantiation).
// Callees: std::__insertion_sort 0x0081C4F0 and std::__unguarded_insertion_sort_aux 0x0081A940 (both __fastcall, out of line).
// Shape: the landed __final_insertion_sort 0x004BB520 model: STLport 4.6 with the inline `__unguarded_insertion_sort` passing
//   `_STLP_VALUE_TYPE` = `(_Tp*)0` (`push 0`).
enum EHeroMorphType { };
class CParticleMorphs {
public:
    class CEntry {                                      // 36 bytes in retail
    public:
        unsigned char Data[36];
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;
};
}
template <class Key, class Value> class CKeyPairCompareLess { };   // empty; passed in a 4-byte slot
template <class T> class CCompareString { };                     // empty; passed in a 4-byte slot

typedef std::pair<EHeroMorphType, CParticleMorphs::CEntry> TElem;
typedef CKeyPairCompareLess<EHeroMorphType, CParticleMorphs::CEntry> TCompare;

namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);              // 0x0081C4F0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x0081A940

template <class _Iterator> struct iterator_traits {
    typedef typename _Iterator::value_type value_type;
};
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
};

template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __comp);   // _STLP_VALUE_TYPE
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}

template void __fastcall __final_insertion_sort(TElem*, TElem*, TCompare);
}