// std::__adjust_heap(CVertexGroupBlockOptimiser::CBlockInfo* first, int hole, int len, CBlockInfo value, less<CBlockInfo> comp) -- STLport heap sift-down of the block candidates by cost, then __push_heap @ 0x00ADAAA0
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAVCBlockInfo@CVertexGroupBlockOptimiser@@HV12@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@YIXPAVCBlockInfo@CVertexGroupBlockOptimiser@@HHV12@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADB000, 150 bytes, `ret 0x14`.
// Callee: 0x00ADAAA0 __push_heap (same instantiation; manifest `_Push_heap<CVertexGroupBlockOptimiser::CBlockInfo*,int,
// CBlockInfo>`).  __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then len, the 12-byte block info by value
// and the empty 4-byte comparator.
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8) with
// CBlockInfo::operator< on Cost (signed `jge`); the Ego_r.pdb name set holds the CBlockInfo heap family
// (_Adjust_heap/_Push_heap/_Make_heap over PAVCBlockInfo@CVertexGroupBlockOptimiser). The propagated catalog label
// pair<EExpressionType,CExpressionSet::CEntry> is wrong: the key is the dword at +4.
// Namespace: `std`, as the applied rename1 siblings of this family (0x00ADBE70 / 0x00ADC080 / 0x00ADAB20).
// Shape: STLport 4.x `__adjust_heap`; the bottom case reuses the child index (`first[idx - 1]`).
namespace std {

template <class _Tp> struct less {
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00ADAAA0

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    for (; __secondChild < __len; __secondChild = 2 * __secondChild + 2) {
        if (__comp(__first[__secondChild], __first[__secondChild - 1]))
            --__secondChild;
        __first[__holeIndex] = __first[__secondChild], __holeIndex = __secondChild;
    }
    if (__secondChild == __len) {
        __first[__holeIndex] = __first[__secondChild - 1];
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __value, __comp);
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
template void __adjust_heap<CVertexGroupBlockOptimiser::CBlockInfo*, int, CVertexGroupBlockOptimiser::CBlockInfo,
                            less<CVertexGroupBlockOptimiser::CBlockInfo> >(
    CVertexGroupBlockOptimiser::CBlockInfo*, int, int, CVertexGroupBlockOptimiser::CBlockInfo, less<CVertexGroupBlockOptimiser::CBlockInfo>);
}
