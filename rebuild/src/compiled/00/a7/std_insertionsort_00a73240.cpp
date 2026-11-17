// std::__insertion_sort(std::pair<long,C3DMeshVolume>* first, std::pair<long,C3DMeshVolume>* last, CKeyPairCompareLess<long,C3DMeshVolume> comp) -- straight insertion sort: hand each entry after the first by value to __linear_insert (the CPhysicsMesh navigation containment volumes (CVectorMap<long, C3DMeshVolume>))
// Leaf: ??$__insertion_sort@PAU?$pair@JVC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A73240, 109 bytes (__fastcall: first / last in ecx / edx, the rest pushed).
// Identity: the CPhysicsMesh navigation containment volumes (CVectorMap<long, C3DMeshVolume>) sort family (std::sort 0x00A73B20, NF91 q11191 / q11192). Callees include 0x00A728F0.
// NF91 (2026-11-17, claimed as the q11191 / q11192 family follow-up): the old row spelt the element as a row-local `class pair` (PAV) on local
//   CCharString / C3DMeshVolume / CKeyPairCompareLess models (own leaf ??$__insertion_sort@PAV?$pair@JVC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIXPAV?$pair@JVC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z); STLport's pair is a struct (PAU), as the sort front end
//   now binds it. Now the STLport 4.6 body of stlport/fable_containers.h by explicit instantiation over engine/C3DMeshVolume.h and the global
//   comparator of engine/CVectorMap.h (the 0x00A34D10 recipe); `// Flags: /Gr`. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_INSERTION_SORT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::__insertion_sort(std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,CKeyPairCompareLess<long,C3DMeshVolume>);
