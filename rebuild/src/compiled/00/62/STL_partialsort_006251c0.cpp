// _STL::__partial_sort(CTestQuest* first, CTestQuest* middle, CTestQuest* last, CTestQuest*, CTestQuestNameCompare comp) -- STLport partial_sort core for the test-quest list: heapify [first, middle), pop every smaller later element into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006251C0, length 225 bytes, `ret 0Ch`.
// Callers: 0x0062532C (__introsort_loop's depth-limit fallback) and 0x006255A0 (the partial_sort front end).
// Callees: __make_heap(first, last, comp, T*, int*) @ 0x00624880 (landed), CTestQuestNameCompare::operator() @ 0x006238A0
//   (out of line, thiscall on the comparator slot), CCharString copy constructor @ 0x0099EC30 (x6: the by-value
//   T(*i) built in the argument area by CTestQuest's implicit copy constructor), __pop_heap @ 0x00624800 (landed),
//   sort_heap @ 0x00624FC0 (landed).
// Label: catalog "std::introsort_partition_impl" is descriptive; the body is STLport 4.x `__partial_sort` verbatim:
//   `make_heap(__first, __middle, __comp); for (__i = __middle; __i < __last; ++__i) if (__comp(*__i, *__first))
//   __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __DISTANCE_TYPE(__first, _RandomAccessIter));
//   sort_heap(__first, __middle, __comp);`
// Shape: /Gr algorithm TU (`__fastcall` template: first in ecx, middle in edx, last, the value-type tag and the 4-byte
//   comparator on the stack); make_heap's two null tags pushed before the comparator; the loop runs on a precomputed
//   trip count ((last - middle - 1) / 28 + 1, kept in the dead `last` slot); first is spilled to the `push ecx` word.
//   The comparator word rides in esi across the compare only when operator() is a same-TU noinline body that leaves
//   *this alone (declaration-only, VC7.1 reloads the slot after every compare: 225v225 with ten register/reload diffs).
// Element: CTestQuest (0x1C bytes, PDB member names, as the landed __pop_heap / __make_heap / sort_heap of this TU).
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8

class CCharString {
public:
    struct CData { char* PData; };
    CData* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }
};

class CTestQuest {
public:
    CCharString QuestName;                         // +0x00
    CCharString HSPName;                           // +0x04
    long        ColourIndex;                       // +0x08
    CCharString DisplayName;                       // +0x0c
    CCharString IniFile;                           // +0x10
    CCharString OutiFile;                          // +0x14
    CCharString QuestCardDefName;                  // +0x18
};

#pragma auto_inline(off)
struct CTestQuestNameCompare {
    // 0x006238A0 (landed): a same-TU noinline stand-in that reads only the two quests, so VC7.1 knows the comparator
    // slot it is called on survives the call (retail keeps the comparator word in esi across it).
    __declspec(noinline) bool operator()(const CTestQuest& a, const CTestQuest& b) const
    {
        return _stricmp(a.DisplayName.GetPString(), b.DisplayName.GetPString()) < 0;
    }
};
#pragma auto_inline(on)

namespace _STL {
template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*);   // 0x00624880
template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result, _Tp __value, _Compare __comp, _Distance*);   // 0x00624800
template <class _RandomAccessIter, class _Compare>
void __fastcall sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00624FC0

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIter, class _Compare>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
}

template void __fastcall _STL::__partial_sort<CTestQuest*, CTestQuest, CTestQuestNameCompare>(CTestQuest*, CTestQuest*, CTestQuest*, CTestQuest*, CTestQuestNameCompare);