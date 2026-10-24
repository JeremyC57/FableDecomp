// _global::_Adjust_heap(CVertexGroupBlockOptimiser::CBlockInfo*, long, long, CVertexGroupBlockOptimiser::CBlockInfo, less<CBlockInfo>) -- heap sift-down of vertex-group block candidates by cost, then _Push_heap @ 0x00ADAAA0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADB000, 150 bytes.
// Callee: 0x00ADAAA0 _Push_heap (manifest: _Push_heap<CVertexGroupBlockOptimiser::CBlockInfo*,int,
// CBlockInfo>, the callee's own label -- it is this row's instantiation).  The propagated label
// pair<EExpressionType,CExpressionSet::CEntry> is wrong: the key is the dword at +4.
// CVertexGroupBlockOptimiser::CBlockInfo (PDB, 12 bytes): BlockIndex +0, Cost +4, Benefit +8; the
// ordering compares Cost (signed `jge`).  Link-time __fastcall: first in ecx, hole in edx, then
// bottom, the 12-byte block info by value (built in the _Push_heap slot word by word through
// `mov eax,esp`: a user-provided copy constructor) and the 4-byte comparator; `ret 0x14`.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
struct CBlockInfo {                             // CVertexGroupBlockOptimiser::CBlockInfo
    unsigned long BlockIndex;                   // +0x0
    long Cost;                                  // +0x4
    long Benefit;                               // +0x8
    CBlockInfo(const CBlockInfo& other) : BlockIndex(other.BlockIndex), Cost(other.Cost), Benefit(other.Benefit) { }
    bool operator<(const CBlockInfo& other) const { return Cost < other.Cost; }
};

struct SBlockInfoLess {                         // std::less<CBlockInfo>
    char m_tag;
    bool operator()(const CBlockInfo& a, const CBlockInfo& b) const { return a < b; }
};

void __fastcall _Push_heap(CBlockInfo* first, long hole, long top, CBlockInfo value, SBlockInfoLess pred);   // 0x00ADAAA0

void __fastcall _Adjust_heap(CBlockInfo* first, long hole, long bottom, CBlockInfo value, SBlockInfoLess pred)
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