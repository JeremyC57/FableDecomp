// std::__copy_backward(std::pair<long, long>* first, std::pair<long, long>* last, std::pair<long, long>* result, const random_access_iterator_tag&, int*) -- assign [first, last) onto the range ending at result, back to front
// Leaf: ??$__copy_backward@PAU?$pair@JJ@std@@PAU12@H@std@@YIPAU?$pair@JJ@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F7E52, 47 bytes, `__fastcall` (first ecx, last edx; result, the tag and the distance
//   pointer on the stack), `ret 0Ch`.
// Identity (proven): bound ??$__copy_backward@PAU?$pair@JJ@std@@PAU12@H@std@@... from the landed __linear_insert<pair<long, long>*> 0x005F9ADB; also called by the landed copy_backward<pair<long, long>*> 0x005F93F6. The manifest's "_Copy_backward<CBrainUpdateZone*, ...>" is a propagated (later-STL) label.
// Shape (family M164, representative 0x005F7E23): STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n)
//   *--__result = *--__last; return __result;` -- the element's implicit assignment is two dword moves. Size TU.
#pragma optimize("s", on)
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template ::std::pair<long, long>* __fastcall __copy_backward< ::std::pair<long, long>*, ::std::pair<long, long>*, int>(::std::pair<long, long>*, ::std::pair<long, long>*, ::std::pair<long, long>*, const random_access_iterator_tag&, int*);
}