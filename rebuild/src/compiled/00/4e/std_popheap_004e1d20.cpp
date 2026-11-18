// std::__pop_heap(pair<CCharString,CTCBase::CRegisteredTCInfo>* first, .. last, .. result, pair<CCharString,CTCBase::CRegisteredTCInfo> value, CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo> comp, int*) -- STLport 4.6 _heap.c pop_heap core of the registered-TC table sort: `*result = *first`, then __adjust_heap(first, 0, last - first, value, comp)
// Flags: /Gr
// Leaf: ??$__pop_heap@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@H@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@00U10@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@PAH@Z
// Retail: Fable.exe @ 0x004E1D20, 72 bytes (`ret 1Ch`: result, the 16-byte value, the comparator and
//         the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Callees: pair<CCharString, CTCBase::CRegisteredTCInfo>::operator= 0x004DA56A (manifest
//          "pair<CCharString,CCharString>" propagated), its copy constructor 0x004D788D (builds the
//          by-value argument in the argument area), __adjust_heap 0x004DFDBB (manifest label),
//          CCharString::~CCharString 0x0099EAE0 (the by-value parameter destroyed by this callee).
// Label: the manifest's "CTCThingOwner::SetOwnedObjectsSerialisationSectionName" is propagated over this
// heap helper of the registered-TC table sort (the landed __pop_heap_aux 0x0059474E is the sibling
// front end for another element type).
// Element: pair<CCharString, CTCBase::CRegisteredTCInfo>, 16 bytes; the info's CCharString sits at
// +0x0C (value destroyed as `lea ecx,[ebp+18h]` then `lea ecx,[ebp+0Ch]`, reverse member order).
// Shape: a frame-pointer body that still uses esi/edi and pushes the comparator straight from its slot
//        (`push dword ptr [ebp+1Ch]`) = `optimize("y",off)` + `("s",on)`; arguments right to left:
//        comp, the value copy (`sub esp,10h ; mov ecx,esp`), len = (last - first) / 16, then
//        holeIndex 0 in edx and first in ecx.
#pragma optimize("y", off)
// NF82 (q10998): moved off the row-local CCharString / CRegisteredTCInfo / SRegisteredTCPair / CKeyPairCompareLess models onto the
//   shared headers, as NF80's 0x004DFDBB (same instantiation): stlport/fable_containers.h (FABLE_RETAIL_STLP_HEAP: the STLport 4.6
//   _heap.c __pop_heap body, __adjust_heap declared only), engine/CTCBase.h (CTCBase::CRegisteredTCInfo), engine/CVectorMap.h
//   (CKeyPairCompareLess) and fable_string.h; `// Flags: /Gr` (the old row spelled __fastcall by hand). The __adjust_heap call now
//   binds the template instance 0x004DFDBB emits (was ?__adjust_heap@@YIXPAUSRegisteredTCPair@@JJU1@UCKeyPairCompareLess@@@Z).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_HEAP
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void std::__pop_heap(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,
 std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>,
 CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>,int*);
