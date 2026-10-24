// _global::_Adjust_heap(pair<EExpressionType,CExpressionSet::CEntry>*, long, long, pair<EExpressionType,CExpressionSet::CEntry>, CKeyPairCompareLess<EExpressionType,CExpressionSet::CEntry>) -- heap sift-down of the expression table by expression type, then _Push_heap @ 0x0086F100
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086F450, 149 bytes.
// Callee: 0x0086F100 _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole in
// edx, then bottom, the 12-byte pair by value and the 4-byte comparator; `ret 0x14`.
// Element (12 bytes): EExpressionType key +0, CExpressionSet::CEntry +4 (PDB, 8 bytes: Type +0,
// AnimIndex +4).  CKeyPairCompareLess compares the keys (signed dword `jge`).  Element assignment
// is a 3-dword block copy (trivial operator=); the by-value pair is built in the _Push_heap slot
// word by word through `mov eax,esp`, the shape of a user-provided copy constructor.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
enum EExpressionType { EXPRESSION_TYPE_NONE };

struct CExpressionSetEntry {                    // CExpressionSet::CEntry
    EExpressionType Type;                       // +0x0
    long AnimIndex;                             // +0x4
};

struct SExpressionPair {                        // std::pair<EExpressionType, CExpressionSet::CEntry>
    EExpressionType first;                      // +0x0
    CExpressionSetEntry second;                 // +0x4
    SExpressionPair(const SExpressionPair& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const SExpressionPair& a, const SExpressionPair& b) const { return a.first < b.first; }
};

void __fastcall _Push_heap(SExpressionPair* first, long hole, long top, SExpressionPair value, CKeyPairCompareLess pred);   // 0x0086F100

void __fastcall _Adjust_heap(SExpressionPair* first, long hole, long bottom, SExpressionPair value, CKeyPairCompareLess pred)
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