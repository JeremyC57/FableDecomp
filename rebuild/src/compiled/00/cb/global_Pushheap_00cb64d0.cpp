// _global::_Push_heap(pair<CCharString,CScriptInfo>*, long, long, pair<CCharString,CScriptInfo>, CKeyPairCompareLess<CCharString,CScriptInfo>) -- heap sift-up of the script registry's (name, script info) pairs
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB64D0, 223 bytes, `ret 0x20`.
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EFB0 CCharString::operator= (x2 per
// move), 0x0099EAE0 ~CCharString (x2 at the end).  Link-time __fastcall (/Gr algorithm TU): first in
// ecx, hole in edx, then top, the 0x18-byte pair by value and the empty comparator (4 + 0x18 + 4 =
// 0x20).  CScriptInfo (PDB, 20 bytes): AllocFunc +0, MasterScriptDataAlloc +4, CCharString Name +8,
// long ID +0xc, bool MasterScript +0x10; its implicit assignment is two dwords, the Name call, a
// dword and a byte; the by-value pair's destructor runs Name then the key.  TU neighbour of the
// landed CVectorMap<CCharString,CScriptInfo>::LowerBound @ 0x00CB65D0.
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

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo { public:
    CScriptBase* (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                              // +0x04
    CCharString Name;                                                                 // +0x08
    long ID;                                                                          // +0x0c
    bool MasterScript;                                                                // +0x10
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, CScriptInfo> TEntry;

template<class K, class V> class CKeyPairCompareLess { public:
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, CScriptInfo> TCompare;

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