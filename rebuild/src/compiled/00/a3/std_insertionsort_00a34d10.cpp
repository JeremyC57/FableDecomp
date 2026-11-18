// std::__insertion_sort(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, CKeyPairCompareLess<unsigned long, C3DMeshVolume> comp) -- straight insertion sort of the mesh-volume table: hand each entry after the first by value to __linear_insert
// Leaf: ??$__insertion_sort@PAU?$pair@KVC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34D10, 109 bytes (+ int3 padding), `ret 4` (__fastcall: first in ecx, last
//   in edx, the empty comparator on the stack).
// Identity: the __final_insertion_sort head of the C3DMeshStats volume-table sort (std::sort 0x00A355F0). Callees:
//   CCharString::CCharString(const CCharString&) 0x0099EC30 (the by-value pair copy), __linear_insert 0x00A34820.
// NF91 (2026-11-17, q11193): moved off the row-local CCharString / C3DMeshVolume / std::pair / std::CKeyPairCompareLess models onto
//   the shared headers (engine/C3DMeshVolume.h, engine/CVectorMap.h, the STLport 4.6 body of stlport/fable_containers.h
//   FABLE_RETAIL_STLP_INSERTION_SORT_BODY, the 0x0077ECB0 recipe) by explicit instantiation; `// Flags: /Gr`. STLport instance
//   name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_INSERTION_SORT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::__insertion_sort(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
