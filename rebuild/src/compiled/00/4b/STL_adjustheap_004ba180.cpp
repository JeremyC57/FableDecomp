// std::__adjust_heap(pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>* first, int holeIndex, int len, pair<CCharString,CPersistInfo> val, CKeyPairCompareLess comp) -- STLport heap sift-down of the quest-persistence (quest name, persist info) table by name, then __push_heap val back up
// Leaf: ??$__adjust_heap@PAU?$pair@VCCharString@@UCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BA180, 282 bytes (+ int3 padding), `ret 14h` (__fastcall: first in ecx,
//   holeIndex in edx; len, the 12-byte value and the empty comparator on the stack: 4 + 0xC + 4 = 0x14).
// Element (12 bytes, `lea eax,[esi+esi*2]` * 4): CCharString key +0, CPersistInfo +4 (Ego_r.pdb, 8 bytes:
//   CCharString QuestName +0, CCharString PersistScript +4).
// Callees: CBasicString<char>::operator< @ 0x00429950 (payload compare of the inline CCharString::operator<),
//   CCharString::operator= @ 0x0099EFB0 x3 (implicit element assignment, walked through one source/target pointer
//   pair), CCharString(const CCharString&) @ 0x0099EC30 x3 (memberwise pair copy built in __push_heap's argument
//   slot), __push_heap @ 0x004B9540 (same instantiation), ~CCharString @ 0x0099EAE0 x3 (val, in reverse).
// Callers: __pop_heap 0x004BABF0 and the make_heap / sort family of the same table.
// Shape: STLport 4.6 `__adjust_heap`; topIndex homed in the `push ecx` slot; `comp(first[second], first[second-1])`.
struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (!PStringData)
            return true;
        if (!other.PStringData)
            return false;
        return *PStringData < *other.PStringData;
    }
private:
    CBasicStringChar* PStringData;
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    struct CPersistInfo {                       // Ego_r.pdb, 8 bytes
        CCharString QuestName;                  // +0x0
        CCharString PersistScript;              // +0x4
    };
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> CPersistPair;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x004B9540

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap(CPersistPair*, int, int, CPersistPair,
                                       CKeyPairCompareLess<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>);
}