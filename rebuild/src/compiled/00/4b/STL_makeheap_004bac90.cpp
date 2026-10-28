// std::__make_heap(pair<CCharString,CPersistInfo>* first, pair<CCharString,CPersistInfo>* last, CKeyPairCompareLess<CCharString,CPersistInfo> comp, pair<CCharString,CPersistInfo>*, int*) -- STLport make_heap over the quest-persistence script map: sift every parent from (len - 2) / 2 down to 0 into place by quest-name key
// Instantiation: std::__make_heap<pair<CCharString,CPersistInfo>*,CKeyPairCompareLess<CCharString,CPersistInfo>,pair<CCharString,CPersistInfo>,int>
// Leaf: ??$__make_heap@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@U12@H@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BAC90, 123 bytes, __fastcall (first ecx, last edx; the comparator slot,
//   the value-type and distance-type pointers on the stack), `ret 0Ch`, no frame pointer.
// Identity: called by the pair<CCharString, CPersistInfo> sort chain's __partial_sort @ 0x004BB810 (from the
//   __introsort_loop @ 0x004BBA40 under sort @ 0x004BBB40 <- CVectorMap<CCharString,
//   NQuestManager::CRuntimeQuestPersistence::CPersistInfo, ..>::Sort @ 0x004BBBC0) and by the unreferenced out-of-line
//   make_heap(first, last, comp) copy @ 0x004BB010. The catalog name `std::Heapify` is descriptive.
// Callees: __adjust_heap(first, holeIndex, len, value, comp) @ 0x004BA180 (out of line), CCharString's copy constructor
//   @ 0x0099EC30 (STLport pair's member-wise copy constructor building the by-value `_Tp(*(first + parent))`).
// Shape: STLport 4.6 `__make_heap`: len by the 0x2AAAAAAB multiply (a divide by 12) kept in ebp, `first` parked in the
//   pushed ecx slot, the parent's `second` addressed through edi stepping 12 bytes down. /Gr TU: the templates are
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
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                      // 0x004BA180

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

template void __fastcall __make_heap(CPersistEntry*, CPersistEntry*, CPersistLess, CPersistEntry*, int*);
}