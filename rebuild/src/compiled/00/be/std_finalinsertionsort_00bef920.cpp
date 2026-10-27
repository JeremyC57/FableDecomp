// std::__final_insertion_sort(std::pair<std::pair<long, long>, CEngineTextureDecal>* __first, std::pair<std::pair<long, long>, CEngineTextureDecal>* __last, CKeyPairCompareLess<std::pair<long, long>, CEngineTextureDecal> __comp) -- STLport: insertion-sort the first 16 elements, then the unguarded pass over the rest (or a plain insertion sort of a short range)
// Leaf: ??$__final_insertion_sort@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@@std@@YIXPAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@0@0V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEF920, 86 bytes, `__fastcall` (first ecx, last edx, the empty comparator on the stack), `ret 4`.
// Identity (proven): its callees are the landed __insertion_sort 0x00BEF640 and __unguarded_insertion_sort_aux 0x00BEE840 over pair<pair<long, long>, CEngineTextureDecal> with CKeyPairCompareLess<pair<long, long>, CEngineTextureDecal> -- the entries of Ego_r.pdb CEngineInternalPrimitiveMorphInfo::TextureDecals (CVectorMap<pair<long,long>, CEngineTextureDecal, ...>), 28 bytes; its caller is that map's sort 0x00BEFC20 (the landed CVectorMap sort 0x00BEFC80 is its sibling).
// Callees: __insertion_sort(first, last, comp) 0x00BEF640, __unguarded_insertion_sort_aux(first, last, T*, comp) 0x00BEE840.
// Shape (family M191, representative 0x0067F0E0): STLport 4.6 _algo.c `__final_insertion_sort` verbatim (the landed CBlockInfo
//   instance 0x00ADC080): `if (__last - __first > __stl_threshold) { __insertion_sort(__first, __first + 16, __comp);
//   __unguarded_insertion_sort(__first + 16, __last, __comp); } else __insertion_sort(__first, __last, __comp);` -- the
//   28-byte stride's `lea esi,[ecx+0x1C0]`. /Gr TU written as explicit __fastcall.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
class CCompositeBlendDesc;
class CEngineTextureDecal {                                // Ego_r.pdb, 20 bytes in retail
public:
    long OriginalTextureIndex;                            // +0x00
    long DecalID;                                         // +0x04
    CCompositeBlendDesc* OverlaidBlends[3];               // +0x08 vector<CCompositeBlendDesc> (STLport, 12 bytes)
};
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00BEF640
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                               _Compare __comp);   // 0x00BEE840
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
template void __fastcall __final_insertion_sort<std::pair<std::pair<long, long>, CEngineTextureDecal>*, CKeyPairCompareLess<std::pair<long, long>, CEngineTextureDecal> >(std::pair<std::pair<long, long>, CEngineTextureDecal>*, std::pair<std::pair<long, long>, CEngineTextureDecal>*, CKeyPairCompareLess<std::pair<long, long>, CEngineTextureDecal>);
}