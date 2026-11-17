// std::__final_insertion_sort<std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> >(first, last, comp) -- STLport 4.6 final pass of the registered-TC table sort: guarded insertion sort of the first 16 entries and unguarded insertion of the rest, or a plain insertion sort of a short range
// Flags: /Gr
// Leaf: ??$__final_insertion_sort@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E415D, 68 bytes, `ret 4` (size-optimised TU: unaligned entry).
// Callees: __insertion_sort(first, last, comp) 0x004E3686 (`__fastcall`, `ret 4`), __unguarded_insertion_sort_aux(first, last, (T*)0, comp)
//   0x004DFD60 (`__fastcall`, `ret 8`) -- both NF82 template instances over this element.
// Identity (NF82 q11074, checked by NF85): its landed caller std::sort 0x004EB9A6 binds this exact spelling (probed), and its callees are the
//   same sort's __insertion_sort / __unguarded_insertion_sort_aux (NF82, CTCBase::CRegisteredTCInfo); the old row emitted the stand-in
//   ?__final_insertion_sort@std@@YIXPAU?$pair@UCCharString@std@@UCRegisteredTCInfo@2@@1@0UCKeyPairCompareLess@1@@Z and bound both callees
//   under matching stand-ins.
// NF85 (q11074): re-authored as the explicit instantiation of the NF85 opt-in FABLE_RETAIL_STLP_FINAL_INSERTION_SORT_BODY
//   (stlport/fable_containers.h: STLport 4.6 _algo.c __final_insertion_sort over the out-of-line helpers) with the element from
//   engine/CTCBase.h (the NF82 0x004E3686 includes).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_FINAL_INSERTION_SORT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void __fastcall std::__final_insertion_sort(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,
 CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>);
