// std::sort(pair<CCharString,CPersistInfo>* first, pair<CCharString,CPersistInfo>* last, CKeyPairCompareLess<CCharString,CPersistInfo> comp) -- STLport introsort of the quest-persistence entries by quest-name key: introsort to depth 2*lg(n), then the final insertion sort
// Instantiation: std::sort<pair<CCharString,CPersistInfo>*,CKeyPairCompareLess<CCharString,CPersistInfo> >
// Leaf: ??$sort@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@std@@YIXPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BBB40, 82 bytes, /Gr __fastcall (first ecx, last edx), `ret 4` (the
//   empty comparator); speed TU (the 12-byte count by `imul` 0x2AAAAAAB).
// Identity (sweep1, 2026-10-25): FINDINGS chunk 457 / hand457 README ("0x004BBB40 sort (catalogued Heap_BuildFromRange /
//   Sort_SElement12)") in the quest-persistence sort chain over pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>
//   (12 bytes; Ego_r.pdb CRuntimeQuestPersistence::PersistScripts / MasterDataPersistScripts are
//   CVectorMap<CCharString, CPersistInfo, CKeyPairCompareLess<CCharString, CPersistInfo> >). Callers: CVectorMap::Sort 0x004BBBC0, the inline SortIfDirty
//   0x004BBBE0 and the landed CRuntimeQuestPersistence::SetQuestScriptPersistScript 0x004B4E30 /
//   SetQuestMasterDataPersistScript 0x004B4F30 (which bind it as "SGI sort<pair*, CKeyPairCompareLess> @ 0x004BBB40").
//   Callees: __introsort_loop 0x004BBA40 (it calls the landed std::__unguarded_partition 0x004BA080 and
//   std::__partial_sort 0x004BB810) and __final_insertion_sort 0x004BB520.
// Shape: STLport 4.6 `sort`: `__introsort_loop(first, last, _STLP_VALUE_TYPE, __lg(last - first) * 2, comp)` then
//   `__final_insertion_sort(first, last, comp)`; the comparator slot is reloaded once into ebx for both calls.
class CCharString {
public:
    void* PStringData;                                      // +0x0 the shared string block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator<(const CCharString& other) const;
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                    // Ego_r.pdb, 8 bytes
    public:
        CCharString QuestName;                              // +0x0
        CCharString PersistScript;                          // +0x4
    };
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo CPersistInfo;
typedef std::pair<CCharString, CPersistInfo> CPersistEntry;
typedef CKeyPairCompareLess<CCharString, CPersistInfo> CPersistLess;

namespace std {
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }   // _STLP_VALUE_TYPE for pointers

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);                                                   // 0x004BBA40
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004BB520

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}

template void __fastcall sort(CPersistEntry*, CPersistEntry*, CPersistLess);
}
