// std::__introsort_loop(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume>*, int depth_limit, CKeyPairCompareLess<unsigned long, C3DMeshVolume> comp) -- introsort driver of the mesh-volume table: median-of-three quicksort partitions down to 16-entry runs, heap-sorting a range once the depth limit runs out
// Leaf: ??$__introsort_loop@PAU?$pair@KVC3DMeshVolume@@@std@@U12@HV?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@00HV?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35490, 234 bytes (+ int3 padding), `ret 0Ch` (__fastcall: first in ecx,
//   last in edx, the value-type pointer, the depth limit and the empty comparator on the stack).
// Identity: the sort of the 16-byte C3DMeshStats table (recursive: it calls itself). Callees: __partial_sort @ 0x00A35260,
//   CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the pivot copy), __unguarded_partition @ 0x00A34260, itself.
// NF89 (2026-11-17, q11174): moved off the row-local CCharString / C3DMeshVolume / std::pair / std::CKeyPairCompareLess / TEntry models onto
//   the shared headers: element STLport pair<unsigned long, C3DMeshVolume> (engine/C3DMeshVolume.h), comparator the global
//   CKeyPairCompareLess<unsigned long, C3DMeshVolume> (engine/CVectorMap.h), body the STLport 4.6 __introsort_loop of
//   stlport/fable_containers.h FABLE_RETAIL_STLP_INTROSORT_LOOP by explicit instantiation (the landed 0x0077F370 recipe; __median inline,
//   partial_sort the inline front end of the out-of-line __partial_sort, __unguarded_partition out of line). It now binds 0x00A35260 with
//   NF87's spelling. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_INTROSORT_LOOP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__introsort_loop(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,int,CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
