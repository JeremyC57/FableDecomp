// std::__unguarded_partition(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, CEngineSubPrimitiveRenderer::CSortElement pivot, std::less<CEngineSubPrimitiveRenderer::CSortElement> comp) -- STLport Hoare partition: advance first past elements below pivot, retreat last past elements above it, swap and continue until they cross; return first
// Leaf: ??$__unguarded_partition@PAVCSortElement@CEngineSubPrimitiveRenderer@@V12@U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@std@@@std@@YIPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@0V12@U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b85430, 85 bytes, /Gr __fastcall (first in ecx, last in edx; pivot, comp on the stack), `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) ??$__unguarded_partition@PAVCSortElement@CEngineSubPrimitiveRenderer@@V12@U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@std@@@std@@...; its only caller is the landed __introsort_loop 0x00B867A0 (CEngineSubPrimitiveRenderer::SortList's CSortElement sort).
// Callees: none (the comparator is inline: an unsigned compare of the first dword).
// Shape (family L054): STLport 4.6 _algo.c __unguarded_partition verbatim with the inline iter_swap (two-dword swap
//   through ebx/esi/edi); the pivot's key stays in ecx.
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
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2> inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}
template CEngineSubPrimitiveRenderer::CSortElement* __fastcall std::__unguarded_partition(CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement, std::less<CEngineSubPrimitiveRenderer::CSortElement>);