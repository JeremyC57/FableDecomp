// std::__unguarded_partition<std::pair<long,C3DMeshVolume>*, CKeyPairCompareLess<long,C3DMeshVolume> > (STLport 4.6) -- quicksort partition step around the by-value pivot's long key: advance from both ends, swap out-of-place entries, return the split point
// Leaf: ??$__unguarded_partition@PAU?$pair@JVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIPAU?$pair@JVC3DMeshVolume@@@0@PAU10@0U10@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71880, 127 bytes, `ret 14h` (__fastcall /Gr algorithm TU: first in ecx, last in edx).
// Callees: the swap 0x00A70320 (out of line, its own row already ??$swap@U?$pair@JVC3DMeshVolume@@@std@@@std@@YI..), operator delete 0x00BFE9BC, ~CCharString 0x0099EAE0 (the by-value pivot).
// NF89 (2026-11-17, q11175): moved off the row-local CCharString / C3DMeshVolume / pair / comparator models onto the shared headers, as NF85 /
//   NF87 did for the rest of the family: element the STLport pair<long, C3DMeshVolume> (engine/C3DMeshVolume.h; NF87's __partial_sort
//   0x00A73330 spells it), comparator the global CKeyPairCompareLess<long, C3DMeshVolume> (engine/CVectorMap.h), body STLport 4.6 by explicit
//   instantiation of stlport/fable_containers.h SORT_INTERNALS (iter_swap = swap(*a, *b), out of line; the landed 0x0077E8F0 recipe). Was the pair<long, SNamedChunk> PAV stand-in spelling. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template std::pair<long,C3DMeshVolume>* FABLE_STLP_FASTCALL std::__unguarded_partition(std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>,CKeyPairCompareLess<long,C3DMeshVolume>);
