// std::__copy_backward_ptrs(pair<long, float>* first, pair<long, float>* last, pair<long, float>* result, const __false_type&) -- STLport backward copy for an 8-byte element with a non-trivial (__false_type) assignment: walk n = last - first elements down from the ends, `*--result = *--last`, and return the new start of the destination
// Leaf: ??$__copy_backward_ptrs@PAU?$pair@JM@std@@PAU12@@std@@YIPAU?$pair@JM@0@PAU10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B5540, 43 bytes, __fastcall (first ecx, last edx), `ret 8`; returns result - n in eax.
// Identity (proven): element `pair<long, float>`; evidence: its only caller is vector<pair<long, float> >::_M_fill_insert 0x00786020 (agent951 G011, proven through its overflow callee 0x006B5BE0 = vector<pair<long, float> >::_M_insert_overflow, the landed vector<pair<long, float> >::insert 0x006B5ED0).
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

template pair<long, float>* __fastcall __copy_backward_ptrs<pair<long, float>*, pair<long, float>*>(pair<long, float>*, pair<long, float>*, pair<long, float>*, const __false_type&);
}