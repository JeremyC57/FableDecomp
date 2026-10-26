// std::__copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*) -- (std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, int) STLport random-access backward copy: n = last - first assignments `*--result = *--last` through the element's out-of-line operator=, return the new result
// Flags: /Gr
// Leaf: ??$__copy_backward@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA738, 47 bytes (size-optimised TU: unaligned entry), __fastcall (/Gr: first ecx,
//   last edx; result, the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy_backward<std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, int>; evidence: the landed std::__linear_insert 0x004E1C8A over the thing-component registry (pair<CCharString, CTCBase::CRegisteredTCInfo>, global CVectorMap 0x013BA89C) calls it for copy_backward(first, last, last + 1), and its element operator= 0x004DA56A is the landed pair<CCharString, CTCBase::CRegisteredTCInfo>::operator=; the other callers are the __copy_backward_ptrs forwarders 0x004DBF08 / 0x004DD860 / 0x004DFDA6.
// Label: the manifest's "_Copy_backward<CArray<unsigned_short>*,CArray<unsigned_short>*>" is a propagated Dinkumware spelling (family J078: five backward copies, 8- or
//   16-byte strides; retail has no identical-code folding).
// Callees: std::pair<CCharString, CTCBase::CRegisteredTCInfo>::operator= 0x004DA56A (landed, J-family pair assignment) (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h random-access __copy_backward verbatim; the count is a shift (0x10-byte elements) and the
//   result pointer is decremented in its own stack slot (the J070 __copy model, backwards).
#pragma optimize("s", on)
class CCharString {                                      // 4 bytes (Ego_r)
public:
    void* PStringData;                                   // +0x0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
#define CTCBase CTCBasePdb
#include "engine/CTCBase.h"
#undef CTCBase
class CThing;
class CTCBase : public CTCBasePdb {                      // engine/CTCBase.h is data-only: the nested record is added here
public:
    class CRegisteredTCInfo {                            // Ego_r.pdb, 12 bytes
    public:
        CTCBase*      (__fastcall* AllocFunc)(CThing&);  // +0x0
        unsigned long DriverType;                        // +0x4
        CCharString   Name;                              // +0x8
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x0
    _T2 second;                                          // +0x4
    pair& operator=(const pair& __o);                    // 0x004DA56A for pair<CCharString, CTCBase::CRegisteredTCInfo> (implicit; out of line in retail)
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
typedef std::pair<CCharString, CTCBase::CRegisteredTCInfo> TElement_004da738;
template TElement_004da738* __copy_backward<TElement_004da738*, TElement_004da738*, int>(TElement_004da738*, TElement_004da738*, TElement_004da738*,
                                                                         const random_access_iterator_tag&, int*);
}