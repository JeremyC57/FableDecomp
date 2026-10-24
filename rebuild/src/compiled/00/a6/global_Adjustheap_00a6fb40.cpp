// _global::_Adjust_heap(pair<CCharString,C3DDummyObject>*, long, long, pair<CCharString,C3DDummyObject>, CCompareString<C3DDummyObject>) -- heap sift-down of named dummy objects by name, then _Push_heap @ 0x00A6F1D0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6FB40, 224 bytes.
// Callees: 0x009E6530 CCharString::FastCompare (out of line), 0x0099EFB0 CCharString::operator=,
// 0x0099EC30 CCharString copy ctor, 0x0099EAE0 ~CCharString, 0x00A6F1D0 _Push_heap (same
// instantiation).  Link-time __fastcall: first in ecx, hole in edx, then bottom, the 0x38-byte pair
// by value and the 4-byte comparator; `ret 0x40`.
// Element (0x38 bytes): CCharString name +0, C3DDummyObject +4 (PDB, 52 bytes: CMatrix3x4 Transform
// +0, long BoneIndex +0x30) -- trivially copyable, so the implicit pair copy/assignment is the
// CCharString call plus `rep movsd` of 13 dwords; edi is needed for the block move, so `first` is
// kept in the frame next to `top`.  The by-value pair is destroyed here at the end.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;      // 0x009E6530
};

struct CMatrix3x4 {
    float M[3][4];
};

struct C3DDummyObject {
    CMatrix3x4 Transform;                                  // +0x00
    long BoneIndex;                                        // +0x30
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, C3DDummyObject> TEntry;

template<class T> struct CCompareString {
    char m_tag;
    bool operator()(const pair<CCharString, T>& a, const pair<CCharString, T>& b) const { return a.first.FastCompare(b.first); }
};
typedef CCompareString<C3DDummyObject> TCompare;

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare pred);   // 0x00A6F1D0

void __fastcall _Adjust_heap(TEntry* first, long hole, long bottom, TEntry value, TCompare pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}