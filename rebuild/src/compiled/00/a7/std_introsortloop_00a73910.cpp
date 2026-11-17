// std::__introsort_loop<std::pair<long,C3DMeshVolume>*, CKeyPairCompareLess<long,C3DMeshVolume> > (STLport 4.6) -- STLport introsort recursion: median-of-three quicksort partitions down to 16 elements, heap sort once the depth budget is spent
// Leaf: ??$__introsort_loop@PAU?$pair@JVC3DMeshVolume@@@std@@U12@HV?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@00HV?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A73910, 234 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx, last in edx).
// Callees: CCharString copy 0x0099EC30 (the pivot), __unguarded_partition 0x00A71880, itself, __partial_sort 0x00A73330 (NF87).
// NF89 (2026-11-17, q11175): moved off the row-local CCharString / C3DMeshVolume / pair / comparator models onto the shared headers, as NF85 /
//   NF87 did for the rest of the family: element the STLport pair<long, C3DMeshVolume> (engine/C3DMeshVolume.h; NF87's __partial_sort
//   0x00A73330 spells it), comparator the global CKeyPairCompareLess<long, C3DMeshVolume> (engine/CVectorMap.h), body STLport 4.6 by explicit
//   instantiation of stlport/fable_containers.h INTROSORT_LOOP (the landed 0x0077F370 recipe; __median inline, partial_sort the inline front end of the out-of-line __partial_sort). Was the pair<long, SNamedChunk> PAV stand-in spelling. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_INTROSORT_LOOP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__introsort_loop(std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,int,CKeyPairCompareLess<long,C3DMeshVolume>);
