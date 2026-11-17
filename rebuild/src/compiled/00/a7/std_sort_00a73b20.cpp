// std::sort<std::pair<long, C3DMeshVolume>*, CKeyPairCompareLess<long, C3DMeshVolume> >(first, last, comp) -- STLport 4.6 introsort of the CPhysicsMesh navigation containment volumes (CVectorMap<long, C3DMeshVolume>'s pairs)
// Leaf: ??$sort@PAU?$pair@JVC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A73B20, 115 bytes (`__fastcall`, `ret 4`).
// Identity: STLport 4.6 _algo.c sort over pair<long, C3DMeshVolume> (16-byte elements): its out-of-line callees are the same instance's
//   __introsort_loop 0x00A73910 (NF89), __insertion_sort 0x00A73240 and __unguarded_insertion_sort_aux 0x00A71090; its callers are
//   CVectorMap<long, C3DMeshVolume>::Sort 0x00A73C40 and ::Update 0x00A73C80. Descriptive-by-library: the spelling is what VC7.1
//   emits for STLport 4.6 over the element the callees prove (the landed 0x0077F440 recipe).
// NF91 (2026-11-17, q11191): the old _global::Sort_SElement16_00a73b20 name and its SElement16 / SCompare models were stand-ins; now
//   stlport/fable_containers.h (FABLE_RETAIL_STLP_SORT_FASTCALL + _SORT_FASTCALL_BODY) over engine/C3DMeshVolume.h and engine/CVectorMap.h;
//   explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_SORT_FASTCALL_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::sort<std::pair<long,C3DMeshVolume>*, CKeyPairCompareLess<long,C3DMeshVolume> >(
    std::pair<long,C3DMeshVolume>*, std::pair<long,C3DMeshVolume>*, CKeyPairCompareLess<long,C3DMeshVolume>);
