// std::sort(std::pair<std::pair<long, long>, CEngineTextureDecal>* first, std::pair<std::pair<long, long>, CEngineTextureDecal>* last, CKeyPairCompareLess<std::pair<long, long>, CEngineTextureDecal> comp) -- STLport introsort: the bounded quicksort/heapsort loop, then the final insertion sort
// Leaf: ??$sort@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@@std@@YIXPAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@0@0V?$CKeyPairCompareLess@U?$pair@JJ@std@@VCEngineTextureDecal@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEFC20, 85 bytes, `__fastcall` (first ecx, last edx, the comparator on the stack), `ret 4`.
// Identity (probable): its callers 0x00BEFC80 / 0x00BEFCA0 are a CVectorMap's Sort() / Update() (`sort(First, Last, Compare); Dirty = false` with the empty comparator at +0x0C and Dirty at +0x0D); the 28-byte stride is the pair<pair<long,long>, CEngineTextureDecal> entry of CEngineInternalPrimitiveMorphInfo::TextureDecals (landed LowerBound 0x00BD8480 in the same module), whose sort helpers (_Sort / _Insertion_sort / heap) are in Ego_r.pdb. The manifest's "introsort_and_heapify" / "make_heap_Sound" labels are descriptive.
// Callees: std::__introsort_loop<T*, T, int, Compare> 0x00BEFA20 and std::__final_insertion_sort<T*, Compare> 0x00BEF920 (`__fastcall`).
// Shape (family M170, representative 0x006257C0): STLport 4.6 `if (__first != __last) { __introsort_loop(__first, __last,
//   (_Tp*)0, __lg(__last - __first) * 2, __comp); __final_insertion_sort(__first, __last, __comp); }` -- __lg inline
//   (`for (k = 0; n != 1; n >>= 1) ++k`). Speed TU.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
class CEngineTextureDecal {                              // retail 20 bytes (Ego_r.pdb 24: 16-byte vector)
public:
    unsigned long Members[5];
};
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};
namespace std {
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00BEFA20
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00BEF920
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (std::pair<std::pair<long, long>, CEngineTextureDecal>*)0, __lg((int)(__last - __first)) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort< ::std::pair<std::pair<long, long>, CEngineTextureDecal>*, ::CKeyPairCompareLess<std::pair<long, long>, CEngineTextureDecal> >(::std::pair<std::pair<long, long>, CEngineTextureDecal>*, ::std::pair<std::pair<long, long>, CEngineTextureDecal>*, ::CKeyPairCompareLess<std::pair<long, long>, CEngineTextureDecal>);
}