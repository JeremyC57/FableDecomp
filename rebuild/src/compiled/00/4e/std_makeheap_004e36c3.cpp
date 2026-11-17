// std::make_heap<std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> >(first, last, comp) -- STLport 4.6 make_heap front end of the registered-TC table sort: forward to __make_heap with the (T*)0 and (ptrdiff_t*)0 tags
// Flags: /Gr
// Leaf: ??$make_heap@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E36C3, 16 bytes: `push 0 ; push 0 ; push [esp+0Ch] ; call 0x004E1D68 ; ret 4` -- the out-of-line __fastcall
//   front end (first ecx, last edx, the empty comparator on the stack) of the /Os registry-sort TU (unaligned entry).
// Identity (NF82 q11072, checked by NF85): its only landed caller std::__partial_sort 0x004E6E9E (NF82) binds this exact spelling, and its
//   callee 0x004E1D68 is that sort's std::__make_heap (NF82: ??$__make_heap@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@..),
//   so the element and comparator are the registered-TC table's (CTCBase::CRegisteredTCInfo, Ego_r.pdb). The old row was a generic
//   operator-delete forwarder model copied from 0x005949A5 (catalog CMemoryAllocator::operator_delete; extern "C" _operator_delete@4 calling
//   0x004E1D68 as _inner_free@12): the same three pushes, a different meaning.
// NF85 (q11072): re-authored as the explicit instantiation of the NF85 opt-in FABLE_RETAIL_STLP_MAKE_HEAP_FRONT_BODY (stlport/fable_containers.h:
//   STLport 4.6 _heap.c make_heap over the out-of-line __make_heap) with the element from engine/CTCBase.h (the NF82 0x004E3686 includes).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAKE_HEAP_FRONT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void __fastcall std::make_heap(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,
 CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>);
