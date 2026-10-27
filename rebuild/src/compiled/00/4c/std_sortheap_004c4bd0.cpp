// std::sort_heap(NGraphicAppearance::CMeshInfo* first, NGraphicAppearance::CMeshInfo* last, CSortPrimaryMeshesLast comp) -- (<NGraphicAppearance::CMeshInfo*, CSortPrimaryMeshesLast>) turn the heap [first, last) into a sorted range
// Leaf: ??$sort_heap@PAVCMeshInfo@NGraphicAppearance@@VCSortPrimaryMeshesLast@@@std@@YIXPAVCMeshInfo@NGraphicAppearance@@0VCSortPrimaryMeshesLast@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C4BD0, 154 bytes, `__fastcall` (first ecx, last edx, the empty comparator's slot on the stack), `ret 4`.
// Identity (proven): its callee 0x004C2D10 is bound ??$__adjust_heap@PAVCMeshInfo@NGraphicAppearance@@HV12@VCSortPrimaryMeshesLast@@@std@@...; its caller is the partial_sort 0x004C52D0.
// Callees: std::__adjust_heap<NGraphicAppearance::CMeshInfo*, int, NGraphicAppearance::CMeshInfo, CSortPrimaryMeshesLast> 0x004C2D10 (__fastcall: first ecx, hole 0 in edx, then len, the
//   value and the comparator by value).
// Shape (family M091, representative 0x004C4B30): STLport 4.6 _heap.h `while (__last - __first > 1) pop_heap(__first, __last--,
//   __comp);` with pop_heap -> __pop_heap_aux -> __pop_heap inline (`_Tp(*(__last - 1))`, `*__result = *__first`,
//   `__adjust_heap(__first, 0, __last - __first, __val, __comp)`) over 12-byte trivially copyable elements; speed TU.
namespace NGraphicAppearance {
class CMeshInfo {                                         // Ego_r.pdb, 12 bytes
public:
    long Index;                                           // +0x0
    float SizeX;                                          // +0x4
    unsigned char PrimaryMesh;                            // +0x8 (bit-field)
};
}
class CSortPrimaryMeshesLast {
public:
    bool operator()(const NGraphicAppearance::CMeshInfo& a, const NGraphicAppearance::CMeshInfo& b) const;
};
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x004C2D10
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
    __pop_heap_aux(__first, __last, (NGraphicAppearance::CMeshInfo*)0, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<NGraphicAppearance::CMeshInfo*, CSortPrimaryMeshesLast >(NGraphicAppearance::CMeshInfo*, NGraphicAppearance::CMeshInfo*, CSortPrimaryMeshesLast);
}