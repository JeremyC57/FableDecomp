// _global::_Push_heap(pair<CCharString,CSymbolMap::CSymbolInfo>*, long, long, pair<CCharString,CSymbolMap::CSymbolInfo>, CCompareString<CSymbolMap::CSymbolInfo>) -- heap sift-up of the symbol map's (name, symbol info) pairs
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E67C0, 205 bytes, `ret 0x18`.
// Callees: 0x009E6530 CCharString::FastCompare, 0x0099EFB0 CCharString::operator= (x3 per move),
// 0x0099EAE0 ~CCharString (x3 at the end).  Link-time __fastcall (/Gr algorithm TU): first in ecx,
// hole in edx, then top, the 16-byte pair by value and the empty comparator (4 + 0x10 + 4 = 0x18).
// CSymbolMap::CSymbolInfo (PDB, 12 bytes): CCharString GroupName +0, CCharString EntryName +4, long
// Val +8; the implicit pair assignment is three CCharString calls and a dword.
// CCompareString<T>: `a.first.FastCompare(b.first)` (CCharString::FastCompare @ 0x009E6530, out of line).
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;      // 0x009E6530
};

class CSymbolMap { public:
    class CSymbolInfo { public:
        CCharString GroupName;                             // +0x00
        CCharString EntryName;                             // +0x04
        long Val;                                          // +0x08
    };
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, CSymbolMap::CSymbolInfo> TEntry;

template<class T> class CCompareString { public:
    char m_tag;
    bool operator()(const pair<CCharString, T>& a, const pair<CCharString, T>& b) const { return a.first.FastCompare(b.first); }
};
typedef CCompareString<CSymbolMap::CSymbolInfo> TCompare;

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