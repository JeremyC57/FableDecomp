// std::__introsort_loop<std::pair<CCharString,C3DMeshVolume>*, CKeyPairCompareLess<CCharString,C3DMeshVolume> > (STLport 4.6) -- STLport introsort recursion: median-of-three quicksort partitions down to 16 elements, heap sort once the depth budget is spent
// Leaf: ??$__introsort_loop@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@U12@HV?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DMeshVolume@@@0@00HV?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A73A00, 216 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx, last in edx).
// Callees: __median 0x00A70020 (out of line: the CCharString-keyed median is past VC7.1's inline budget), CCharString copies 0x0099EC30 (the pivot), __unguarded_partition 0x00A71910, itself, __partial_sort 0x00A733B0 (NF87).
// NF89 (2026-11-17, q11176): moved off the row-local CCharString / C3DMeshVolume / pair / comparator models onto the shared headers, as NF85 /
//   NF87 did for the rest of the family: element the STLport pair<CCharString, C3DMeshVolume> (engine/C3DMeshVolume.h; NF87's __partial_sort
//   0x00A733B0 spells it), comparator the global CKeyPairCompareLess<CCharString, C3DMeshVolume> (engine/CVectorMap.h), body STLport 4.6 by explicit
//   instantiation of stlport/fable_containers.h INTROSORT_LOOP (the landed 0x0077F370 recipe; partial_sort the inline front end of the out-of-line __partial_sort). Was the pair<CCharString, SNamedChunk> PAV stand-in spelling. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_INTROSORT_LOOP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__introsort_loop(std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,int,CKeyPairCompareLess<CCharString,C3DMeshVolume>);
