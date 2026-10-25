// _STL::__unguarded_linear_insert(pair<CCharString,CPersistInfo>* last, pair<CCharString,CPersistInfo> val, CKeyPairCompareLess<CCharString,CPersistInfo> comp) -- STLport unguarded insertion step of the quest-persistence script map: walk down from `last` shifting every entry whose quest name sorts after val's, then store val
// Instantiation: _STL::__unguarded_linear_insert<pair<CCharString,CPersistInfo>*,pair<CCharString,CPersistInfo>,CKeyPairCompareLess<CCharString,CPersistInfo> >
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@_STL@@U12@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B9480, 148 bytes, __fastcall (last in ecx; the 12-byte by-value entry
//   and the 4-byte comparator slot on the stack), `ret 10h`; the callee destroys its by-value entry.
// Identity: the element is NQuestManager::CRuntimeQuestPersistence's CVectorMap<CCharString, CPersistInfo,
//   CKeyPairCompareLess<..> > entry, pair<CCharString, CPersistInfo{QuestName, PersistScript}> (12 bytes): the sort chain
//   sort @ 0x004BBB40 <- CVectorMap::Sort @ 0x004BBBC0 / SortIfDirty @ 0x004BBBE0 (FableWin.pdb has the matching
//   `CVectorMap<CCharString, CPersistInfo, ..>::Sort` and sort/_Insertion_sort instantiations). Called by
//   __unguarded_insertion_sort_aux @ 0x004BA100 and __linear_insert @ 0x004BAB30. The catalog name
//   `std::set_Insert_KeyValue` is wrong.
// Callees: CBasicString<char>::operator< @ 0x00429950 (the inlined CCharString::operator< null checks around it),
//   CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0.
// Shape: STLport 4.6 `__unguarded_linear_insert`; the pair / CPersistInfo assignments are the implicit member-wise ones
//   (three operator= calls), val's implicit destructor runs PersistScript, QuestName, first. /Gr TU: the template is
//   `__fastcall`. The landed 0x00429950 body sits above under `auto_inline(off)` as the extra rule asks.
#pragma auto_inline(off)
template <class T> class CBasicString {
public:
    T* m_pData;                                             // +0x0
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    // 0x00429950 (landed): the real body as a same-TU noinline function (STLport-over-CCharString rule).
    __declspec(noinline) bool operator<(const CBasicString& other) const { return Compare(other) < 0; }
};
#pragma auto_inline(on)

class CCharString {
public:
    CBasicString<char>* PStringData;                        // +0x0 the shared string block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
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

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const _STL::pair<Key, Value>& a, const _STL::pair<Key, Value>& b) const { return a.first < b.first; }
};

typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo CPersistInfo;
typedef _STL::pair<CCharString, CPersistInfo> CPersistEntry;
typedef CKeyPairCompareLess<CCharString, CPersistInfo> CPersistLess;

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template void __fastcall __unguarded_linear_insert(CPersistEntry*, CPersistEntry, CPersistLess);
}