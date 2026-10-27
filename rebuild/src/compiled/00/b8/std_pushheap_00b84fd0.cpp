// std::__push_heap(CEngineSubPrimitiveRenderer::CSortElement* first, int holeIndex, int topIndex, CEngineSubPrimitiveRenderer::CSortElement value, std::less<CEngineSubPrimitiveRenderer::CSortElement> comp) -- STLport heap sift-up: move parents down while they compare below value, then drop value into the hole
// Leaf: ??$__push_heap@PAVCSortElement@CEngineSubPrimitiveRenderer@@HV12@U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@std@@@std@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@HHV12@U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b84fd0, 79 bytes, /Gr __fastcall (first in ecx, holeIndex in edx; topIndex, value, comp on the stack), `ret 10h`.
// Identity (proven): its only caller is the landed __adjust_heap 0x00B852F0, under the __make_heap 0x00B855F0 / sort_heap 0x00B85E10 / __partial_sort 0x00B86450 bound to CEngineSubPrimitiveRenderer::CSortElement with less<CSortElement> (CEngineSubPrimitiveRenderer::SortList's sort of a CArray<CSortElement>); 8-byte element {unsigned long SortValue; CEngineSubPrimitiveBase* SubPrimitive}; bound only as the stand-in ?_Push_heap@FableHeap_00b852f0@@....
// Callees: none (the comparator is inline: an unsigned compare of the first dword).
// Shape (family L053): STLport 4.6 _heap.c __push_heap verbatim -- parent = (hole - 1) / 2 (`cdq ; sub ; sar`), the
//   8-byte element moved as two dwords, value's key kept in ebx.
class CEngineSubPrimitiveBase;
class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                   // Ego_r.pdb, 8 bytes
    public:
        unsigned long SortValue;                          // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;            // +0x04
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
}
template void __fastcall std::__push_heap(CEngineSubPrimitiveRenderer::CSortElement*, int, int, CEngineSubPrimitiveRenderer::CSortElement, std::less<CEngineSubPrimitiveRenderer::CSortElement>);