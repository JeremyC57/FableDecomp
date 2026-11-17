// std::sort<std::pair<unsigned long, C3DMeshVolume>*, CKeyPairCompareLess<unsigned long, C3DMeshVolume> >(first, last, comp) -- STLport 4.6 introsort of the C3DMeshStats volume table (CVectorMap<unsigned long, C3DMeshVolume>'s pairs)
// Leaf: ??$sort@PAU?$pair@KVC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A355F0, 115 bytes (`__fastcall`, `ret 4`).
// Identity: STLport 4.6 _algo.c sort over pair<unsigned long, C3DMeshVolume> (16-byte elements): its out-of-line callees are the same instance's
//   __introsort_loop 0x00A35490 (NF89), __insertion_sort 0x00A34D10 and __unguarded_insertion_sort_aux 0x00A33870; its callers are
//   CVectorMap<unsigned long, C3DMeshVolume>::Sort 0x00A35670 and ::Update 0x00A35690. Descriptive-by-library: the spelling is what VC7.1
//   emits for STLport 4.6 over the element the callees prove (the landed 0x0077F440 recipe).
// NF91 (2026-11-17, q11190): the old _global::Sort_SElement16_00a355f0 name and its SElement16 / SCompare models were stand-ins; now
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
template void __fastcall std::sort<std::pair<unsigned long,C3DMeshVolume>*, CKeyPairCompareLess<unsigned long,C3DMeshVolume> >(
    std::pair<unsigned long,C3DMeshVolume>*, std::pair<unsigned long,C3DMeshVolume>*, CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
