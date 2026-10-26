// std::__copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*) -- (std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, int) STLport random-access backward copy: n = last - first assignments `*--result = *--last` through the element's out-of-line operator=, return the new result
// Flags: /Gr
// Leaf: ??$__copy_backward@PAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@PAU12@H@std@@YIPAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005930C4, 47 bytes (size-optimised TU: unaligned entry), __fastcall (/Gr: first ecx,
//   last edx; result, the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy_backward<std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, std::pair<long, CDefPointer<const CContextSensitiveItemDef> >*, int>; evidence: the landed std::__linear_insert 0x00594946 over the context-sensitive item priority pairs (pair<long, CDefPointer<const CContextSensitiveItemDef> >, CContextSensitivePriorityCompare) calls it, and its element operator= 0x0059308A is that pair's landed assignment; the other callers are the forwarders 0x005939C7 / 0x00593EBB / 0x005946F1.
// Label: the manifest's "_Copy_backward<CArray<unsigned_short>*,CArray<unsigned_short>*>" is a propagated Dinkumware spelling (family J078: five backward copies, 8- or
//   16-byte strides; retail has no identical-code folding).
// Callees: std::pair<long, CDefPointer<const CContextSensitiveItemDef> >::operator= 0x0059308A (landed) (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h random-access __copy_backward verbatim; the count is a shift (0x8-byte elements) and the
//   result pointer is decremented in its own stack slot (the J070 __copy model, backwards).
#pragma optimize("s", on)
class CDefPointeeBase;
class CContextSensitiveItemDef;
template <class T> class CDefPointer {                   // Ego_r.pdb 4 bytes
public:
    CDefPointeeBase* Object;                             // +0x0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x0
    _T2 second;                                          // +0x4
    pair& operator=(const pair& __o);                    // 0x0059308A for pair<long, CDefPointer<const CContextSensitiveItemDef> > (landed)
};
}
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > TElement_005930c4;
template TElement_005930c4* __copy_backward<TElement_005930c4*, TElement_005930c4*, int>(TElement_005930c4*, TElement_005930c4*, TElement_005930c4*,
                                                                         const random_access_iterator_tag&, int*);
}