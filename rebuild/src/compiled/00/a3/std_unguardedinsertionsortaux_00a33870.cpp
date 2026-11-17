// std::__unguarded_insertion_sort_aux(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume>*, CKeyPairCompareLess<unsigned long, C3DMeshVolume> comp) -- insertion-sort pass with no lower-bound test over the mesh-volume table, each entry handed by value to __unguarded_linear_insert
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@KVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@00V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A33870, 79 bytes (+ int3 padding), `ret 8` (__fastcall: first in ecx, last
//   in edx, the value-type tag pointer and the empty comparator on the stack).
// Identity: the __final_insertion_sort tail of the C3DMeshStats volume-table sort (std::sort 0x00A355F0). Callees:
//   CCharString::CCharString(const CCharString&) 0x0099EC30 (the by-value pair copy), __unguarded_linear_insert 0x00A33250.
// NF91 (2026-11-17, q11194): moved off the row-local CCharString / C3DMeshVolume / std::pair / std::CKeyPairCompareLess models onto
//   the shared headers (engine/C3DMeshVolume.h, engine/CVectorMap.h, the STLport 4.6 body of stlport/fable_containers.h
//   FABLE_RETAIL_STLP_UNGUARDED_INSERTION_SORT_AUX_BODY, the 0x004DFD60 recipe) by explicit instantiation; `// Flags: /Gr`.
//   STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_UNGUARDED_INSERTION_SORT_AUX_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::__unguarded_insertion_sort_aux(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
