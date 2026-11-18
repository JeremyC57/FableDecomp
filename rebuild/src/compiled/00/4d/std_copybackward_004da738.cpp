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
// NF82 (q11001): moved off the row-local CCharString / pair models and the renaming-macro include of engine/CTCBase.h (`#define CTCBase
//   CTCBasePdb` with a local CTCBase on top) onto the shared headers: stlport/fable_containers.h (FABLE_RETAIL_STLP_SORT_INTERNALS: the
//   STLport 4.6 _algobase.h __copy_backward body, explicit instantiation), engine/CTCBase.h (FABLE_RETAIL_CTCBASE_REGISTRY: the retail
//   CTCBase::CRegisteredTCInfo) and fable_string.h. The own symbol is unchanged; NF80's 0x004E1C8A binds it.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
template std::pair<CCharString,CTCBase::CRegisteredTCInfo>* FABLE_STLP_FASTCALL std::__copy_backward(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,
 std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,const std::random_access_iterator_tag&,int*);
