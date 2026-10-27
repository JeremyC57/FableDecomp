// std::__final_insertion_sort(std::pair<unsigned long, C3DDummyObject>* __first, std::pair<unsigned long, C3DDummyObject>* __last, CKeyPairCompareLess<unsigned long, C3DDummyObject> __comp) -- STLport: insertion-sort the first 16 elements, then the unguarded pass over the rest (or a plain insertion sort of a short range)
// Leaf: ??$__final_insertion_sort@PAU?$pair@KVC3DDummyObject@@@std@@V?$CKeyPairCompareLess@KVC3DDummyObject@@@@@std@@YIXPAU?$pair@KVC3DDummyObject@@@0@0V?$CKeyPairCompareLess@KVC3DDummyObject@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A34780, 86 bytes, `__fastcall` (first ecx, last edx, the empty comparator on the stack), `ret 4`.
// Identity (proven): its callees are the landed insertion sort 0x00A34150 and __unguarded_insertion_sort_aux 0x00A323D0 of the C3DMeshStats dummy-object table (pair<unsigned long, C3DDummyObject>, 56 bytes, unsigned key compare: CKeyPairCompareLess); its caller is that sort's driver 0x00A351B0.
// Callees: __insertion_sort(first, last, comp) 0x00A34150, __unguarded_insertion_sort_aux(first, last, T*, comp) 0x00A323D0.
// Shape (family M191, representative 0x0067F0E0): STLport 4.6 _algo.c `__final_insertion_sort` verbatim (the landed CBlockInfo
//   instance 0x00ADC080): `if (__last - __first > __stl_threshold) { __insertion_sort(__first, __first + 16, __comp);
//   __unguarded_insertion_sort(__first + 16, __last, __comp); } else __insertion_sort(__first, __last, __comp);` -- the
//   56-byte stride's `lea esi,[ecx+0x380]`. /Gr TU written as explicit __fastcall.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject {                                      // Ego_r.pdb, 0x34 bytes
public:
    CMatrix3x4 Transform;                                 // +0x00
    long BoneIndex;                                       // +0x30
};
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00A34150
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                               _Compare __comp);   // 0x00A323D0
template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (_Tp*)0, __comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __first, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template void __fastcall __final_insertion_sort<std::pair<unsigned long, C3DDummyObject>*, CKeyPairCompareLess<unsigned long, C3DDummyObject> >(std::pair<unsigned long, C3DDummyObject>*, std::pair<unsigned long, C3DDummyObject>*, CKeyPairCompareLess<unsigned long, C3DDummyObject>);
}