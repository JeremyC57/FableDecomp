// _global::_Push_heap(pair<CCharString,C3DHelperPoint>*, long, long, pair<CCharString,C3DHelperPoint>, CCompareString<C3DHelperPoint>) -- heap sift-up of named helper points by name
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6F0F0, 181 bytes, `ret 0x1c`.
// Callees: 0x009E6530 CCharString::FastCompare, 0x0099EFB0 CCharString::operator=, 0x0099EAE0 ~CCharString.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 0x14-byte pair by
// value and the empty comparator (4 + 0x14 + 4 = 0x1c); the by-value pair is destroyed at the end.
// Element: CCharString name +0, C3DHelperPoint +4 (PDB, 16 bytes: C3DVector Position +0, long
// BoneIndex +0xc) -- the implicit pair assignment is the CCharString call plus four dword moves
// through the re-based source/destination pointers (`add ebx,4 ; add esi,4`).  TU neighbour of the
// C3DDummyObject instantiation 0x00A6F1D0 (adjacent, same TU).
// CCompareString<T>: `a.first.FastCompare(b.first)` (CCharString::FastCompare @ 0x009E6530, out of line).
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;      // 0x009E6530
};

struct C3DVector {
    float X, Y, Z;
};

struct C3DHelperPoint {
    C3DVector Position;                                    // +0x00
    long BoneIndex;                                        // +0x0c
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, C3DHelperPoint> TEntry;

template<class T> struct CCompareString {
    char m_tag;
    bool operator()(const pair<CCharString, T>& a, const pair<CCharString, T>& b) const { return a.first.FastCompare(b.first); }
};
typedef CCompareString<C3DHelperPoint> TCompare;

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