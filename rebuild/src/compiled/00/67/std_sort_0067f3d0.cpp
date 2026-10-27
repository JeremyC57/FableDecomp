// std::sort(CDegradableInfo* first, CDegradableInfo* last, CDegradableDef::CIsHealth1GTHealth2 comp) -- STLport introsort: the bounded quicksort/heapsort loop, then the final insertion sort
// Leaf: ??$sort@PAVCDegradableInfo@@VCIsHealth1GTHealth2@CDegradableDef@@@std@@YIXPAVCDegradableInfo@@0VCIsHealth1GTHealth2@CDegradableDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067F3D0, 85 bytes, `__fastcall` (first ecx, last edx, the comparator on the stack), `ret 4`.
// Identity (proven): its only caller is CDegradableDef::Transfer 0x0067E290; Ego_r.pdb sorts CDegradableDef's vector<CDegradableInfo> with std::sort<..., CDegradableDef::CIsHealth1GTHealth2> (??$_Sort@PAVCDegradableInfo@@HVCIsHealth1GTHealth2@CDegradableDef@@@std@@...); 28-byte stride = Ego_r.pdb CDegradableInfo. The manifest's "introsort_and_heapify" / "make_heap_Sound" labels are descriptive.
// Callees: std::__introsort_loop<T*, T, int, Compare> 0x0067F280 and std::__final_insertion_sort<T*, Compare> 0x0067F0E0 (`__fastcall`).
// Shape (family M170, representative 0x006257C0): STLport 4.6 `if (__first != __last) { __introsort_loop(__first, __last,
//   (_Tp*)0, __lg(__last - __first) * 2, __comp); __final_insertion_sort(__first, __last, __comp); }` -- __lg inline
//   (`for (k = 0; n != 1; n >>= 1) ++k`). Speed TU.
class CDegradableInfo {                                  // Ego_r.pdb, 28 bytes
public:
    unsigned long Members[7];
};
class CDegradableDef {
public:
    class CIsHealth1GTHealth2 {                          // empty comparator (Ego_r.pdb)
    public:
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const;
    };
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
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x0067F280
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0067F0E0
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (CDegradableInfo*)0, __lg((int)(__last - __first)) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort< ::CDegradableInfo*, ::CDegradableDef::CIsHealth1GTHealth2 >(::CDegradableInfo*, ::CDegradableInfo*, ::CDegradableDef::CIsHealth1GTHealth2);
}