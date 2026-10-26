// std::__copy_backward_ptrs(CUnprovenElement_004cbf40* first, CUnprovenElement_004cbf40* last, CUnprovenElement_004cbf40* result, const __false_type&) -- STLport backward copy for an 8-byte element with a non-trivial (__false_type) assignment: walk n = last - first elements down from the ends, `*--result = *--last`, and return the new start of the destination
// Leaf: ??$__copy_backward_ptrs@PAVCUnprovenElement_004cbf40@@PAV1@@std@@YIPAVCUnprovenElement_004cbf40@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CBF40, 43 bytes, __fastcall (first ecx, last edx), `ret 8`; returns result - n in eax.
// Identity (descriptive): element `CUnprovenElement_004cbf40`; evidence: DESCRIPTIVE -- no caller anywhere in the exe (an unreferenced COMDAT: the TU's caller was inlined or its COMDAT copy came from another TU); it sits in the CThing component-map TU after __adjust_heap 0x004CBED0, but 0x004CC790 is a second unreferenced copy in that TU and 15 more unreferenced copies exist elsewhere, so the element is not evidenced.
//   Family H004 (7 rows; the same 43-byte body as agent950's F013, 37 byte-identical copies in the exe, one COMDAT per element
//   type: retail has no identical-code folding). The manifest label "_gapscan::sub_*" / "_Copy_impl<pair<float,
//   EConversationAttitude>*,...>" is a placeholder / propagated Dinkumware spelling.
//   STLport 4.6 _algobase.h: the non-trivial-assignment `__copy_backward_ptrs` that vector::_M_fill_insert / insert call directly
//   with their `_TrivialAss()` tag, the random-access `__copy_backward(first, last, result, random_access_iterator_tag(),
//   (ptrdiff_t*)0)` inlined; `__fastcall` on the template definition (agent950 F013 0x00527CA0's model).
// Shape: the element's compiler-generated assignment is an 8-byte block copy (two dwords through edx); the loop keeps
//   n in esi and addresses the source through the constant distance last - result (`mov ecx,edx ; sub ecx,eax`).
class CUnprovenElement_004cbf40 {                  // descriptive: 8 bytes, type unproven
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

template CUnprovenElement_004cbf40* __fastcall __copy_backward_ptrs<CUnprovenElement_004cbf40*, CUnprovenElement_004cbf40*>(CUnprovenElement_004cbf40*, CUnprovenElement_004cbf40*, CUnprovenElement_004cbf40*, const __false_type&);
}