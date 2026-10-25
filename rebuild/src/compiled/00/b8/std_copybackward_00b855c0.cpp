// std::copy_backward(_BidirectionalIter1 first, _BidirectionalIter1 last, _BidirectionalIter2 result) -- (CEngineSubPrimitiveRenderer::CSortElement*) STLport backward copy for an 8-byte element with a non-trivial (__false_type) assignment: walk n = last - first elements down from the ends, `*--result = *--last`, and return the new start of the destination
// Leaf: ??$copy_backward@PAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@@std@@YIPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@00@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B855C0, 43 bytes (+ int3 padding), __fastcall (first ecx, last edx), `ret 4`; returns result - n in eax.
// Identity: element `CEngineSubPrimitiveRenderer::CSortElement`; evidence: its only caller is the landed std::__merge_backward<CSortElement*, ...> 0x00B859A0 (the CEngineSubPrimitiveRenderer stable_sort chain), whose source names this callee copy_backward<CSortElement*, CSortElement*>.
//   Family F013 (15 bodies, one COMDAT per element type; 14 `ret 8` __copy_backward_ptrs + this family's `ret 4`
//   copy_backward 0x00B855C0): the manifest label "_Copy_impl<std::pair<float,EConversationAttitude>*,std::pair<float,enum_EConversationAttitude>*>" is
//   propagated across the byte twins (an Ego_r Dinkumware spelling; no row of the family is proven to hold that pair).
//   STLport 4.6 _algobase.h: copy_backward(first, last, result) with __copy_backward_aux (both pointers: __true_type), __copy_backward_ptrs (non-trivial assignment: __false_type) and the random-access __copy_backward all inlined into the out-of-line `__fastcall` COMDAT the merge calls in its loop exits.
// Shape: the element's compiler-generated assignment is an 8-byte block copy (two dwords through edx); the loop keeps
//   n in esi and addresses the source through the constant distance last - result (`mov ecx,edx ; sub ecx,eax`).
class CEngineSubPrimitiveBase;
class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                     // Ego_r: 8 bytes
    public:
        unsigned long SortValue;                             // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;               // +0x04
        CSortElement() { }
    };
};
namespace std {
struct __true_type { __true_type() { } };
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
inline _BidirectionalIter __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                               _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_aux(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                               _BidirectionalIter2 __result, const __true_type&)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __fastcall copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result)
{
    return __copy_backward_aux(__first, __last, __result, __true_type());
}

template CEngineSubPrimitiveRenderer::CSortElement* __fastcall copy_backward<CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*>(CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*);
}