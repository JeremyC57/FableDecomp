// std::copy_backward(std::pair<long, long>* first, std::pair<long, long>* last, std::pair<long, long>* result) -- STLport copy_backward over a pair array: forward to __copy_backward with the iterator category and a null distance pointer
// Leaf: ??$copy_backward@PAU?$pair@JJ@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F93F6, 21 bytes, `__fastcall` (/Gr: first in ecx, last in edx passed through), `ret 4`, frame pointer.
// Identity (proven): its callee 0x005F7E52 is bound ??$__copy_backward@PAU?$pair@JJ@std@@PAU12@H@std@@YI.... No call, jump or reference reaches this COMDAT; the three-argument wrapper (one stack
//   argument) is copy_backward.
// Callee: __copy_backward(first, last, result, const random_access_iterator_tag&, int*) 0x005F7E52 (__fastcall).
// Shape (family K120, representative 0x004DFDA6): STLport 4.6 `copy_backward` -> `__copy_backward(..., random_access_iterator_tag(),
//   (ptrdiff_t*)0)`; frame/size TU, the empty tag homed in the top byte of the result argument slot ([ebp+0Bh]).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class _T1, class _T2> struct pair;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x005F7E52
template <class _BI1, class _BI2>
inline _BI2 __fastcall copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template std::pair<long, long>* __fastcall copy_backward<std::pair<long, long>*, std::pair<long, long>*>(std::pair<long, long>*, std::pair<long, long>*, std::pair<long, long>*);
}