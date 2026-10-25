// std::__copy_backward_ptrs(_RandomAccessIter first, _RandomAccessIter last, _BidirectionalIter result, const __false_type&) -- (CUnprovenElement_006e49d0*) STLport backward copy for an 8-byte element with a non-trivial (__false_type) assignment: walk n = last - first elements down from the ends, `*--result = *--last`, and return the new start of the destination
// Leaf: ??$__copy_backward_ptrs@PAVCUnprovenElement_006e49d0@@PAV1@@std@@YIPAVCUnprovenElement_006e49d0@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E49D0, 43 bytes (+ int3 padding), __fastcall (first ecx, last edx), `ret 8`; returns result - n in eax.
// Identity: element `CUnprovenElement_006e49d0` (descriptive: the element type is a placeholder); evidence: its callers are the fill insert 0x006E5240 behind three resize bodies; the one with a caller, 0x006E3EC0 (reached from CWorld::Init), resizes a static vector at 0x013BABA4 to 3 entries and stores two floats per entry (both words are read with fld): C2DVector or a pair of floats, not proven.
//   Family F013 (15 bodies, one COMDAT per element type; 14 `ret 8` __copy_backward_ptrs + this family's `ret 4`
//   copy_backward 0x00B855C0): the manifest label "_Copy_impl<std::pair<float,EConversationAttitude>*,std::pair<float,enum_EConversationAttitude>*>" is
//   propagated across the byte twins (an Ego_r Dinkumware spelling; no row of the family is proven to hold that pair).
//   STLport 4.6 _algobase.h: the non-trivial-assignment `__copy_backward_ptrs` that vector::_M_fill_insert / insert call directly with their `_TrivialAss()` tag, the random-access `__copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined; `__fastcall` on the template definition.
// Shape: the element's compiler-generated assignment is an 8-byte block copy (two dwords through edx); the loop keeps
//   n in esi and addresses the source through the constant distance last - result (`mov ecx,edx ; sub ecx,eax`).
class CUnprovenElement_006e49d0 {                          // descriptive: 8 bytes, type unproven
public:
    unsigned long Word0;
    unsigned long Word1;
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

template CUnprovenElement_006e49d0* __fastcall __copy_backward_ptrs<CUnprovenElement_006e49d0*, CUnprovenElement_006e49d0*>(CUnprovenElement_006e49d0*, CUnprovenElement_006e49d0*, CUnprovenElement_006e49d0*, const __false_type&);
}