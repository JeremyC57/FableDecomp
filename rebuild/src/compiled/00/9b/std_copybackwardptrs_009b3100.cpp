// std::__copy_backward_ptrs(pair<unsigned long, long>* first, pair<unsigned long, long>* last, pair<unsigned long, long>* result, const __false_type&) -- STLport backward copy for an 8-byte element with a non-trivial (__false_type) assignment: walk n = last - first elements down from the ends, `*--result = *--last`, and return the new start of the destination
// Leaf: ??$__copy_backward_ptrs@PAU?$pair@KJ@std@@PAU12@@std@@YIPAU?$pair@KJ@0@PAU10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B3100, 43 bytes, __fastcall (first ecx, last edx), `ret 8`; returns result - n in eax.
// Identity (proven): element `pair<unsigned long, long>`; evidence: its callers are 0x00A397D0 (family H018: vector<pair<unsigned long, long> >::_M_fill_insert, whose overflow callee 0x009B61F0 is agent950's proven vector<pair<unsigned long, long> >::_M_insert_overflow) and 0x00A39610, the same vector's insert(pos, x).
//   Family H004 (7 rows; the same 43-byte body as agent950's F013, 37 byte-identical copies in the exe, one COMDAT per element
//   type: retail has no identical-code folding). The manifest label "_gapscan::sub_*" / "_Copy_impl<pair<float,
//   EConversationAttitude>*,...>" is a placeholder / propagated Dinkumware spelling.
//   STLport 4.6 _algobase.h: the non-trivial-assignment `__copy_backward_ptrs` that vector::_M_fill_insert / insert call directly
//   with their `_TrivialAss()` tag, the random-access `__copy_backward(first, last, result, random_access_iterator_tag(),
//   (ptrdiff_t*)0)` inlined; `__fastcall` on the template definition (agent950 F013 0x00527CA0's model).
// Shape: the element's compiler-generated assignment is an 8-byte block copy (two dwords through edx); the loop keeps
//   n in esi and addresses the source through the constant distance last - result (`mov ecx,edx ; sub ecx,eax`).

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
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

template pair<unsigned long, long>* __fastcall __copy_backward_ptrs<pair<unsigned long, long>*, pair<unsigned long, long>*>(pair<unsigned long, long>*, pair<unsigned long, long>*, pair<unsigned long, long>*, const __false_type&);
}