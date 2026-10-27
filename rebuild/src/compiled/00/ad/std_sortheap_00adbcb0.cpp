// std::sort_heap(CVertexGroupBlockOptimiser::CBlockInfo* first, CVertexGroupBlockOptimiser::CBlockInfo* last, std::less<CVertexGroupBlockOptimiser::CBlockInfo> comp) -- (<CVertexGroupBlockOptimiser::CBlockInfo*, std::less<CVertexGroupBlockOptimiser::CBlockInfo>>) turn the heap [first, last) into a sorted range
// Leaf: ??$sort_heap@PAVCBlockInfo@CVertexGroupBlockOptimiser@@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@YIXPAVCBlockInfo@CVertexGroupBlockOptimiser@@0U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADBCB0, 154 bytes, `__fastcall` (first ecx, last edx, the empty comparator's slot on the stack), `ret 4`.
// Identity (proven): its callee 0x00ADB000 is bound ??$__adjust_heap@PAVCBlockInfo@CVertexGroupBlockOptimiser@@HV12@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@...; its caller is the partial_sort 0x00ADBF00.
// Callees: std::__adjust_heap<CVertexGroupBlockOptimiser::CBlockInfo*, int, CVertexGroupBlockOptimiser::CBlockInfo, std::less<CVertexGroupBlockOptimiser::CBlockInfo>> 0x00ADB000 (__fastcall: first ecx, hole 0 in edx, then len, the
//   value and the comparator by value).
// Shape (family M091, representative 0x004C4B30): STLport 4.6 _heap.h `while (__last - __first > 1) pop_heap(__first, __last--,
//   __comp);` with pop_heap -> __pop_heap_aux -> __pop_heap inline (`_Tp(*(__last - 1))`, `*__result = *__first`,
//   `__adjust_heap(__first, 0, __last - __first, __val, __comp)`) over 12-byte trivially copyable elements; speed TU.
class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {                                    // Ego_r.pdb, 12 bytes
    public:
        unsigned long BlockIndex;                         // +0x0
        long Cost;                                        // +0x4
        long Benefit;                                     // +0x8
        bool operator<(const CBlockInfo& other) const;
    };
};
namespace std {
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
}
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00ADB000
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}
template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, (CVertexGroupBlockOptimiser::CBlockInfo*)0, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<CVertexGroupBlockOptimiser::CBlockInfo*, std::less<CVertexGroupBlockOptimiser::CBlockInfo> >(CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, std::less<CVertexGroupBlockOptimiser::CBlockInfo>);
}