// std::__final_insertion_sort(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp) -- finish an introsort: insertion-sort the first 16 elements, then the unguarded tail
// Leaf: ??$__final_insertion_sort@PAVCTestQuest@@VCTestQuestNameCompare@@@std@@YIXPAVCTestQuest@@0VCTestQuestNameCompare@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00624F60, 86 bytes, `__fastcall` (first ecx, last edx, the comparator's slot on the stack), `ret 4`.
// Identity (proven): its callees are the landed __insertion_sort<CTestQuest*, CTestQuestNameCompare> 0x00624C90 and __unguarded_insertion_sort_aux 0x00624330 of the same instantiation; its caller is that sort's introsort driver 0x006257C0.
// Callees: std::__insertion_sort 0x00624C90, std::__unguarded_insertion_sort_aux 0x00624330 (both __fastcall).
// Shape (family M093, representative 0x004D1AE0): STLport 4.6 _algo.c `if (__last - __first > __stl_threshold) {
//   __insertion_sort(__first, __first + __stl_threshold, __comp); __unguarded_insertion_sort(__first + __stl_threshold, __last,
//   __comp); } else __insertion_sort(__first, __last, __comp);` with __unguarded_insertion_sort inline (its null _Tp* tag).
class CTestQuest {                                        // Ego_r.pdb, 0x1C bytes
public:
    unsigned long Members[7];                             // QuestName, HSPName, ColourIndex, DisplayName, IniFile, OutiFile, QuestCardDefName
};
class CTestQuestNameCompare {
public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;
};
namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00624C90
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x00624330
template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (CTestQuest*)0, __comp);
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
template void __fastcall __final_insertion_sort<CTestQuest*, CTestQuestNameCompare >(CTestQuest*, CTestQuest*, CTestQuestNameCompare);
}