// std::__unguarded_partition(pair<CCharString,CPersistInfo>* first, pair<CCharString,CPersistInfo>* last, pair<CCharString,CPersistInfo> pivot, CKeyPairCompareLess<CCharString,CPersistInfo> comp) -- STLport Hoare partition of the quest-persistence script map around a by-value pivot (ordered by quest-name key): returns the first entry of the upper part
// Instantiation: std::__unguarded_partition<pair<CCharString,CPersistInfo>*,pair<CCharString,CPersistInfo>,CKeyPairCompareLess<CCharString,CPersistInfo> >
// Leaf: ??$__unguarded_partition@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BA080, 128 bytes, __fastcall (first ecx, last edx; the 12-byte by-value
//   pivot and the 4-byte comparator slot on the stack), `ret 10h`; the callee destroys its by-value pivot.
// Identity: called by the __introsort_loop @ 0x004BBA40 of the pair<CCharString, CPersistInfo> sort (sort @ 0x004BBB40,
//   CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo, ..>::Sort @ 0x004BBBC0). The catalog
//   name `std::Introsort_Partition` is descriptive.
// Callees: CBasicString<char>::operator< @ 0x00429950 (inside the inlined CCharString::operator<), swap @ 0x004B7800
//   (out of line, __fastcall, reached through the inline STLport iter_swap), ~CCharString @ 0x0099EAE0.
// Shape: STLport 4.6 `__unguarded_partition`: the pivot key is re-read from its argument slot at every outer pass, the
//   pivot's implicit destructor (PersistScript, QuestName, first) runs before the return. /Gr TU: the templates are
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
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b);                                   // 0x004B7800 (out of line)

// STLport 4.6 iter_swap, inline: the partition calls swap(*__i1, *__i2) directly; the out-of-line iter_swap
// copy 0x004B86A0 is a bare `jmp` to swap.
template <class _ForwardIter1, class _ForwardIter2>
inline void __fastcall iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2) { swap(*__i1, *__i2); }

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template CPersistEntry* __fastcall __unguarded_partition(CPersistEntry*, CPersistEntry*, CPersistEntry, CPersistLess);
}