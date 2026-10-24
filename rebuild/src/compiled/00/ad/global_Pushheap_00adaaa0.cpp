// _global::_Push_heap(CVertexGroupBlockOptimiser::CBlockInfo*, long, long, CVertexGroupBlockOptimiser::CBlockInfo, less<CBlockInfo>) -- heap sift-up of vertex-group block candidates by cost
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADAAA0, 109 bytes, `ret 0x14`.  No callees.
// Called by the landed _Adjust_heap @ 0x00ADB000 (same instantiation; its model is reused here).
// CVertexGroupBlockOptimiser::CBlockInfo (PDB, 12 bytes): BlockIndex +0, Cost +4, Benefit +8; the
// ordering is CBlockInfo::operator< on Cost (`cmp [parent+4],value.Cost ; jge` = signed less).
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 12-byte record
// by value and the empty 4-byte std::less on the stack (4 + 12 + 4 = 0x14).  value.Cost is loaded
// into ebx once at entry (it is the loop's only value read); element moves are three dword copies
// (the implicit operator=); the final drop stores the three value words.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace N00adaaa0 {
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

void __fastcall _Push_heap(CBlockInfo* first, long hole, long top, CBlockInfo value, SBlockInfoLess comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}