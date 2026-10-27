// std::copy_backward(std::pair<long, CDefPointer<const CContextSensitiveItemDef> >* first, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >* last, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >* result) -- STLport copy_backward over a pair array: forward to __copy_backward with the iterator category and a null distance pointer
// Leaf: ??$copy_backward@PAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005946F1, 21 bytes, `__fastcall` (/Gr: first in ecx, last in edx passed through), `ret 4`, frame pointer.
// Identity (probable): its callee 0x005930C4 is the pair<long, CDefPointer<const CContextSensitiveItemDef> > __copy_backward (bound stand-in ?__copy_backward@std@@YIPAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@1@...). No call, jump or reference reaches this COMDAT; the three-argument wrapper (one stack
//   argument) is copy_backward.
// Callee: __copy_backward(first, last, result, const random_access_iterator_tag&, int*) 0x005930C4 (__fastcall).
// Shape (family K120, representative 0x004DFDA6): STLport 4.6 `copy_backward` -> `__copy_backward(..., random_access_iterator_tag(),
//   (ptrdiff_t*)0)`; frame/size TU, the empty tag homed in the top byte of the result argument slot ([ebp+0Bh]).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CContextSensitiveItemDef;
template <class T> class CDefPointer;
namespace std {
template <class _T1, class _T2> struct pair;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x005930C4
template <class _BI1, class _BI2>
inline _BI2 __fastcall copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template std::pair<long, CDefPointer<const CContextSensitiveItemDef> >* __fastcall copy_backward<std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*>(std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*);
}