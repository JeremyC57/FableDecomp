// std::__unguarded_insertion_sort_aux(CTestQuest* first, CTestQuest* last, CTestQuest*, CTestQuestNameCompare comp) -- unguarded insertion sort of [first, last): each element, copied by value, goes through the unguarded linear insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00624330, 126 bytes, __fastcall (first ecx, last edx; the null value-type pointer and
//   the comparator on the stack, `ret 8`).
// Callers: 0x00624757 and 0x00624F9A (the CTestQuest sort's __final_insertion_sort; the TU also holds
//   _Adjust_heap / _Push_heap<CTestQuest*, int, CTestQuest, CTestQuestNameCompare> 0x006243D0 / 0x00623F80).
// Callees: __unguarded_linear_insert(last, val, comp) 0x00623E90, CCharString copy constructor 0x0099EC30.
// Element: Ego_r.pdb CTestQuest (0x1C bytes): CCharString QuestName +0x0, HSPName +0x4, long ColourIndex +0x8, CCharString
//   DisplayName +0xC, IniFile +0x10, OutiFile +0x14, QuestCardDefName +0x18 -- implicit memberwise copy constructor;
//   the comparator is Ego_r.pdb CTestQuestNameCompare (an empty functor, `bool operator()(const CTestQuest&, const
//   CTestQuest&) const`) used for CTCInventoryQuests::GetTestQuestsToDisplay.
// Label: catalog "vector<...>::_Destroy" is propagated; the body is STLport's sort helper.
// Shape: STLport 4.6 `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the
//   by-value element is copy-constructed in its outgoing argument slot (the copy expanded inline); the comparator's
//   operator() stays declaration-only (never called here).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;               // +0x0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

class CTestQuest {                              // Ego_r.pdb, 0x1C bytes
public:
    CCharString QuestName;                      // +0x00
    CCharString HSPName;                        // +0x04
    long        ColourIndex;                    // +0x08
    CCharString DisplayName;                    // +0x0C
    CCharString IniFile;                        // +0x10
    CCharString OutiFile;                       // +0x14
    CCharString QuestCardDefName;               // +0x18
};

class CTestQuestNameCompare {
public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00623E90

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(CTestQuest*, CTestQuest*, CTestQuest*, CTestQuestNameCompare);
}