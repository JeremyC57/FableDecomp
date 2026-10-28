// std::__final_insertion_sort(pair<CCharString,CPersistInfo>* first, pair<CCharString,CPersistInfo>* last, CKeyPairCompareLess<CCharString,CPersistInfo> comp) -- STLport sort tail over the quest-persistence entries: insertion-sort the first 16, unguarded insertion for the rest
// Instantiation: std::__final_insertion_sort<pair<CCharString,CPersistInfo>*,CKeyPairCompareLess<CCharString,CPersistInfo> >
// Leaf: ??$__final_insertion_sort@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@std@@YIXPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BB520, 80 bytes, /Gr __fastcall (first ecx, last edx), `ret 4` (the
//   empty comparator); speed TU (`imul` by 0x2AAAAAAB for the 12-byte count).
// Identity (sweep1, 2026-10-25): hand457 README lists 0x004BB520 `__final_insertion_sort` in the quest-persistence sort chain over pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>
//   (12 bytes; Ego_r.pdb CRuntimeQuestPersistence::PersistScripts / MasterDataPersistScripts are
//   CVectorMap<CCharString, CPersistInfo, CKeyPairCompareLess<CCharString, CPersistInfo> >).
//   Its only caller is the chain's sort 0x004BBB40 (call at 0x004BBB87, after __introsort_loop 0x004BBA40); its callees
//   are the landed std::__insertion_sort 0x004BAFB0 and std::__unguarded_insertion_sort_aux 0x004BA100 (hand457 README).
//   The landed placeholder FinalInsertionSort_SElement12_004bb520 (a generated 12-byte stand-in) had the right body.
// Shape: STLport 4.6 `__final_insertion_sort` with the inline `__unguarded_insertion_sort` passing `_STLP_VALUE_TYPE`
//   = `(_Tp*)0`; threshold 16 entries (`lea edi,[ecx+0C0h]`).
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
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);              // 0x004BAFB0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x004BA100

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

template void __fastcall __final_insertion_sort(CPersistEntry*, CPersistEntry*, CPersistLess);
}
