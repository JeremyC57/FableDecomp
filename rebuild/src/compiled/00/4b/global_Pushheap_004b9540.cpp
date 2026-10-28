// _global::_Push_heap(pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, long, long, pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>, CKeyPairCompareLess<CCharString,CPersistInfo>) -- heap sift-up of the quest persistence registry
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B9540, 209 bytes, `ret 0x14`.
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EFB0 CCharString::operator= (x3 per
// move), 0x0099EAE0 ~CCharString (x3 at the end).  Link-time __fastcall (/Gr algorithm TU): first in
// ecx, hole in edx, then top, the 12-byte pair by value and the empty comparator (4 + 0xc + 4 = 0x14).
// NQuestManager::CRuntimeQuestPersistence::CPersistInfo (PDB, 8 bytes): CCharString QuestName +0,
// CCharString PersistScript +4; the implicit pair assignment is three CCharString calls.
// CCharString::operator< (inline, the landed 0x009B5C70 / 0x00CB68C0 shape): same payload -> false,
// null this -> true, null other -> false, else CBasicString<char>::operator< @ 0x00429950 on the payloads.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

class CCharString { public:
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
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
};

namespace NQuestManager {
struct CRuntimeQuestPersistence {
    class CPersistInfo { public:
        CCharString QuestName;                             // +0x00
        CCharString PersistScript;                         // +0x04
    };
};
}

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> TEntry;

template<class K, class V> class CKeyPairCompareLess { public:
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> TCompare;

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}