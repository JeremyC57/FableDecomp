// std::__push_heap(CVertexGroupBlockOptimiser::CBlockInfo* first, int hole, int top, CBlockInfo value, less<CBlockInfo> comp) -- STLport heap sift-up of the block candidates by cost
// Leaf: ??$__push_heap@PAVCBlockInfo@CVertexGroupBlockOptimiser@@HV12@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@YIXPAVCBlockInfo@CVertexGroupBlockOptimiser@@HHV12@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADAAA0, 109 bytes, `ret 0x14`.  No callees.
// Called by the landed __adjust_heap<CBlockInfo*, ..> 0x00ADB000 (same instantiation).
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8); the
// ordering is CBlockInfo::operator< on Cost (`cmp [parent+4],value.Cost ; jge` = signed less); manifest and Ego_r.pdb
// name set `_Push_heap<CVertexGroupBlockOptimiser::CBlockInfo*,int,CBlockInfo>` (the PDB build's Dinkumware spelling;
// retail is STLport). __fastcall (STLport algorithm TU): first in ecx, hole in edx, then top, the 12-byte record by value
// and the empty 4-byte less<CBlockInfo> on the stack (4 + 12 + 4 = 0x14).  value.Cost is loaded into ebx once at entry
// (the loop's only value read); element moves are three dword copies; the final drop stores the three value words.
// Namespace: `std`, as the applied rename1 siblings of this family (0x00ADBE70 / 0x00ADC080 / 0x00ADAB20).
// Shape: STLport 4.x `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace std {

template <class _Tp> struct less {
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(__first[__parent], __val)) {
        __first[__holeIndex] = __first[__parent];
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    __first[__holeIndex] = __val;
}

} // namespace std

class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {                       // Ego_r.pdb, 12 bytes
    public:
        unsigned long BlockIndex;            // +0x0
        long          Cost;                  // +0x4
        long          Benefit;               // +0x8
        bool operator<(const CBlockInfo& other) const { return Cost < other.Cost; }
    };
};

namespace std {
template void __fastcall __push_heap<CVertexGroupBlockOptimiser::CBlockInfo*, int, CVertexGroupBlockOptimiser::CBlockInfo,
                                     less<CVertexGroupBlockOptimiser::CBlockInfo> >(
    CVertexGroupBlockOptimiser::CBlockInfo*, int, int, CVertexGroupBlockOptimiser::CBlockInfo, less<CVertexGroupBlockOptimiser::CBlockInfo>);
}
