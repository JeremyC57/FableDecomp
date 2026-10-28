// _global::_Push_heap(pair<CCharString,C3DDummyObject>*, long, long, pair<CCharString,C3DDummyObject>, CCompareString<C3DDummyObject>) -- heap sift-up of named dummy objects by name
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6F1D0, 147 bytes, `ret 0x40`.
// Called by the landed _Adjust_heap @ 0x00A6FB40 (same instantiation; its model is reused here).
// Callees: 0x009E6530 CCharString::FastCompare, 0x0099EFB0 CCharString::operator=, 0x0099EAE0 ~CCharString.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 0x38-byte pair by
// value and the empty comparator (4 + 0x38 + 4 = 0x40); the by-value pair is destroyed at the end.
// Element: CCharString name +0, C3DDummyObject +4 (PDB, 52 bytes: CMatrix3x4 Transform +0, long
// BoneIndex +0x30) -- trivially copyable, so the implicit pair assignment is the CCharString call
// plus `rep movsd` x13.
// CCompareString<T>: `a.first.FastCompare(b.first)` (CCharString::FastCompare @ 0x009E6530, out of line).
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;      // 0x009E6530
};

class CMatrix3x4 { public:
    float M[3][4];
};

class C3DDummyObject { public:
    CMatrix3x4 Transform;                                  // +0x00
    long BoneIndex;                                        // +0x30
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, C3DDummyObject> TEntry;

template<class T> class CCompareString { public:
    char m_tag;
    bool operator()(const pair<CCharString, T>& a, const pair<CCharString, T>& b) const { return a.first.FastCompare(b.first); }
};
typedef CCompareString<C3DDummyObject> TCompare;

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