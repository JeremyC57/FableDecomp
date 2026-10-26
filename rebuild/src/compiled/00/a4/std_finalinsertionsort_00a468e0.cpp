// std::__final_insertion_sort(first, last, comp) -- (C3DSkeletalBoneMorph elements, bool (__fastcall*)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)) STLport sort tail: insertion-sort the first 16 elements and run the unguarded insertion over the rest, or insertion-sort a short range whole
// Leaf: ??$__final_insertion_sort@PAVC3DSkeletalBoneMorph@@P6I_NABV1@0@Z@std@@YIXPAVC3DSkeletalBoneMorph@@0P6I_NABV1@1@Z@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A468E0, 81 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx), `ret 4`
//   (the 4-byte comparator); speed TU (`imul` reciprocal for the 20-byte count, threshold `lea edi,[ecx+140h]`).
// Identity (proven): __final_insertion_sort<C3DSkeletalBoneMorph*, bool (__fastcall*)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)>; evidence: its __unguarded_insertion_sort_aux callee 0x00A45F90 is the landed C3DSkeletalBoneMorph instance with a __fastcall function-pointer comparator (C3DSkeletalMorphTarget::Initialise 0x00A45150 sorts its bone morphs), and its only caller is that sort 0x00A46FC0; Ego_r.pdb names the Dinkumware twin _Insertion_sort1<C3DSkeletalBoneMorph*, bool (__fastcall*)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)>.
// Label: the manifest's "sub_00a468e0" is propagated across family H023 (seven bodies that differ only in the element size and the
//   call displacements; retail has no identical-code folding, so each address is one instantiation).
// Callees: std::__insertion_sort 0x00A46680 and std::__unguarded_insertion_sort_aux 0x00A45F90 (both __fastcall, out of line).
// Shape: the landed __final_insertion_sort 0x004BB520 model: STLport 4.6 with the inline `__unguarded_insertion_sort` passing
//   `_STLP_VALUE_TYPE` = `(_Tp*)0` (`push 0`).
class C3DSkeletalBoneMorph {                    // 20 bytes (Ego_r.pdb: BoneName, BoneNameCRC, ...)
public:
    unsigned char Data[20];
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;
};
}
template <class Key, class Value> class CKeyPairCompareLess { };   // empty; passed in a 4-byte slot
template <class T> class CCompareString { };                     // empty; passed in a 4-byte slot

typedef C3DSkeletalBoneMorph TElem;
typedef bool (__fastcall* TCompare)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&);

namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);              // 0x00A46680

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x00A45F90

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