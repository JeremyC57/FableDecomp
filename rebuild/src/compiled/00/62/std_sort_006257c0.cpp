// std::sort(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp) -- STLport introsort: the bounded quicksort/heapsort loop, then the final insertion sort
// Leaf: ??$sort@PAVCTestQuest@@VCTestQuestNameCompare@@@std@@YIXPAVCTestQuest@@0VCTestQuestNameCompare@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006257C0, 85 bytes, `__fastcall` (first ecx, last edx, the comparator on the stack), `ret 4`.
// Identity (proven): its only caller is CTCInventoryQuests::GetTestQuestsToDisplay(vector<CTestQuest>&) 0x0061A8A0 (Ego_r.pdb); Ego_r.pdb sorts that vector with std::sort<..., CTestQuestNameCompare> (??$_Sort@PAVCTestQuest@@HVCTestQuestNameCompare@@@std@@...); 28-byte stride = Ego_r.pdb CTestQuest. The manifest's "introsort_and_heapify" / "make_heap_Sound" labels are descriptive.
// Callees: std::__introsort_loop<T*, T, int, Compare> 0x00625440 and std::__final_insertion_sort<T*, Compare> 0x00624F60 (`__fastcall`).
// Shape (family M170, representative 0x006257C0): STLport 4.6 `if (__first != __last) { __introsort_loop(__first, __last,
//   (_Tp*)0, __lg(__last - __first) * 2, __comp); __final_insertion_sort(__first, __last, __comp); }` -- __lg inline
//   (`for (k = 0; n != 1; n >>= 1) ++k`). Speed TU.
class CTestQuest {                                       // Ego_r.pdb, 28 bytes
public:
    unsigned long Members[7];
};
class CTestQuestNameCompare {                            // empty comparator (Ego_r.pdb)
public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;
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
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00625440
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00624F60
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (CTestQuest*)0, __lg((int)(__last - __first)) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort< ::CTestQuest*, ::CTestQuestNameCompare >(::CTestQuest*, ::CTestQuest*, ::CTestQuestNameCompare);
}