// std::__copy_backward_ptrs(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, CEngineSubPrimitiveRenderer::CSortElement* result, const __false_type&) -- STLport backward copy for an 8-byte element with a non-trivial (__false_type) assignment: walk n = last - first elements down from the ends, `*--result = *--last`, and return the new start of the destination
// Leaf: ??$__copy_backward_ptrs@PAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@@std@@YIPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B850F0, 43 bytes, __fastcall (first ecx, last edx), `ret 8`; returns result - n in eax.
// Identity (proven): element `CEngineSubPrimitiveRenderer::CSortElement`; evidence: its only caller is vector<CEngineSubPrimitiveRenderer::CSortElement>::_M_fill_insert 0x00B85FD0 (family H018), which CEngineSubPrimitiveRenderer::RadixSort(CArray<CSortElement>&) 0x00B847D0 (Ego_r.pdb) calls to grow its sort buffer (this+0x180, Ego_r SortBuffer CArray<CSortElement>) to the argument's size.
//   Family H004 (7 rows; the same 43-byte body as agent950's F013, 37 byte-identical copies in the exe, one COMDAT per element
//   type: retail has no identical-code folding). The manifest label "_gapscan::sub_*" / "_Copy_impl<pair<float,
//   EConversationAttitude>*,...>" is a placeholder / propagated Dinkumware spelling.
//   STLport 4.6 _algobase.h: the non-trivial-assignment `__copy_backward_ptrs` that vector::_M_fill_insert / insert call directly
//   with their `_TrivialAss()` tag, the random-access `__copy_backward(first, last, result, random_access_iterator_tag(),
//   (ptrdiff_t*)0)` inlined; `__fastcall` on the template definition (agent950 F013 0x00527CA0's model).
// Shape: the element's compiler-generated assignment is an 8-byte block copy (two dwords through edx); the loop keeps
//   n in esi and addresses the source through the constant distance last - result (`mov ecx,edx ; sub ecx,eax`).
class CEngineSubPrimitiveBase;
class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // Ego_r.pdb: 8 bytes
    public:
        unsigned long SortValue;
        CEngineSubPrimitiveBase* SubPrimitive;
    };
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter>
inline _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                          _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CEngineSubPrimitiveRenderer::CSortElement* __fastcall __copy_backward_ptrs<CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*>(CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*, const __false_type&);
}