// std::__copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*) -- (CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*, int) STLport random-access backward copy: n = last - first assignments `*--result = *--last` through the element's out-of-line operator=, return the new result
// Flags: /Gr
// Leaf: ??$__copy_backward@PAV?$CIntelligentPointer@VCThing@@@@PAV1@H@std@@YIPAV?$CIntelligentPointer@VCThing@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00655884, 47 bytes (size-optimised TU: unaligned entry), __fastcall (/Gr: first ecx,
//   last edx; result, the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy_backward<CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*, int>; evidence: the landed vector<CIntelligentPointer<CThing> >::_M_fill_insert 0x006562E7 declares and calls this __copy_backward at 0x00655884; the other callers are the forwarder 0x00655E40 and three insert helpers of other TUs (0x008082A0, 0x00808BB0, 0x00809650).
// Label: the manifest's "_Copy_backward<CArray<unsigned_short>*,CArray<unsigned_short>*>" is a propagated Dinkumware spelling (family J078: five backward copies, 8- or
//   16-byte strides; retail has no identical-code folding).
// Callees: CIntelligentPointer<CThing>::operator= 0x004A9760 (landed; binding ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z) (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h random-access __copy_backward verbatim; the count is a shift (0x8-byte elements) and the
//   result pointer is decremented in its own stack slot (the J070 __copy model, backwards).
#pragma optimize("s", on)
class CThing;
class CBaseIntelligentPointer {                          // 8 bytes: vptr + PData
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760 for CThing
};
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
typedef CIntelligentPointer<CThing> TElement_00655884;
template TElement_00655884* __copy_backward<TElement_00655884*, TElement_00655884*, int>(TElement_00655884*, TElement_00655884*, TElement_00655884*,
                                                                         const random_access_iterator_tag&, int*);
}