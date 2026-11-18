// std::__unguarded_partition<std::pair<CCharString,C3DMeshVolume>*, CKeyPairCompareLess<CCharString,C3DMeshVolume> > (STLport 4.6) -- quicksort partition step around the by-value pivot's CCharString key: advance from both ends, swap out-of-place entries, return the split point
// Leaf: ??$__unguarded_partition@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIPAU?$pair@VCCharString@@VC3DMeshVolume@@@0@PAU10@0U10@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z
// Flags: /Gr /d2inlT66
// Inline-budget: /d2inlT66 -- the by-value pivot's implicit ~pair<CCharString, C3DMeshVolume> is called out of line in retail (0x00A6F390, call
//   at 0x00A71971); at the default /d2inlT74 VC7.1 expands it (DIFFER 174 v 110). Sweep: T40 DIFFER(128), T60 / T65 / T66 MATCH, T67 / T68 / T70 /
//   T73 DIFFER(174). T66 is the largest (closest to the default) that matches. Owner decision 2026-11-08, preference (2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71910, 110 bytes, `ret 14h` (__fastcall /Gr algorithm TU: first in ecx, last in edx).
// Callees: CBasicString<char>::operator< 0x00429950 (the inline CCharString::operator< of the comparator), the swap 0x00A70490 (its own row already ??$swap@U?$pair@VCCharString@@VC3DMeshVolume@@@std@@@std@@YI..), ~pair 0x00A6F390 (the by-value pivot).
// NF89 (2026-11-17, q11176): moved off the row-local CCharString / C3DMeshVolume / pair / comparator models onto the shared headers, as NF85 /
//   NF87 did for the rest of the family: element the STLport pair<CCharString, C3DMeshVolume> (engine/C3DMeshVolume.h; NF87's __partial_sort
//   0x00A733B0 spells it), comparator the global CKeyPairCompareLess<CCharString, C3DMeshVolume> (engine/CVectorMap.h), body STLport 4.6 by explicit
//   instantiation of stlport/fable_containers.h SORT_INTERNALS (iter_swap = swap(*a, *b), out of line; the landed 0x0077E8F0 recipe). Was the pair<CCharString, SNamedChunk> PAV stand-in spelling. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template std::pair<CCharString,C3DMeshVolume>* FABLE_STLP_FASTCALL std::__unguarded_partition(std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>,CKeyPairCompareLess<CCharString,C3DMeshVolume>);
