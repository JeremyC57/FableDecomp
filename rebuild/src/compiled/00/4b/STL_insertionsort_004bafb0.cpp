// std::__insertion_sort(pair<CCharString, CPersistInfo>* first, pair<CCharString, CPersistInfo>* last, CKeyPairCompareLess comp) -- STLport insertion sort over the quest manager's persistence table (quest name -> persist info): every entry after the first goes through the out-of-line __linear_insert
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BAFB0, 86 bytes (+ int3 padding), `ret 4` (/Gr algorithm TU, explicit __fastcall
//   here: first in ecx, last in edx, the empty comparator slot on the stack).
// Callers: the landed __final_insertion_sort 0x004BB520 (0x004BB549, 0x004BB567).
// Callees: __linear_insert(first, i, *i, comp) 0x004BAB30 (catalog "std::Set_Insert_OrReplace", propagated),
//   CCharString::CCharString(const CCharString&) 0x0099EC30 (x3).
// Identity: CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo, CKeyPairCompareLess<...> >
//   (Ego_r.pdb ?DeallocateMemory@?$CVectorMap@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@...),
//   12-byte entries; the same element as the landed __pop_heap 0x004BABF0 and sort_heap 0x004BB570. The catalog's
//   "std::vector_ProcessElements" is descriptive.
// Shape: STLport 4.x `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i,
//   comp);` -- the by-value `*i` is built in the argument area by STLport's explicit pair copy constructor: the key
//   string, then CPersistInfo's implicit memberwise copy re-based on its own address (`add edi,4` then `lea ecx,[edi+4]`).
class CCharString {
public:
    void* PStringData;                                  // the shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                // Ego_r.pdb, 8 bytes (implicit copy / assignment)
    public:
        CCharString QuestName;                          // +0x00
        CCharString PersistScript;                      // +0x04
    };
};
}

namespace std {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo TPersistInfo;
typedef std::pair<CCharString, TPersistInfo> TPersistPair;                 // 12 bytes

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;   // only passed along here
};
typedef CKeyPairCompareLess<CCharString, TPersistInfo> TPersistLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x004BAB30

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
template void __fastcall __insertion_sort<TPersistPair*, TPersistLess>(TPersistPair*, TPersistPair*, TPersistLess);
}