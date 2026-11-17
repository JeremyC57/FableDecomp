// std::__unguarded_insertion_sort_aux(std::pair<long,C3DMeshVolume>* first, std::pair<long,C3DMeshVolume>* last, std::pair<long,C3DMeshVolume>*, CKeyPairCompareLess<long,C3DMeshVolume> comp) -- insertion-sort pass with no lower-bound test, each entry handed by value to __unguarded_linear_insert (the CPhysicsMesh navigation containment volumes (CVectorMap<long, C3DMeshVolume>))
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@JVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@00V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71090, 79 bytes (__fastcall: first / last in ecx / edx, the rest pushed).
// Identity: the CPhysicsMesh navigation containment volumes (CVectorMap<long, C3DMeshVolume>) sort family (std::sort 0x00A73B20, NF91 q11191 / q11192). Callees include 0x00A703C0.
// NF91 (2026-11-17, claimed as the q11191 / q11192 family follow-up): the old row spelt the element as a row-local `class pair` (PAV) on local
//   CCharString / C3DMeshVolume / CKeyPairCompareLess models (own leaf ??$__unguarded_insertion_sort_aux@PAV?$pair@JVC3DMeshVolume@@@std@@V12@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIXPAV?$pair@JVC3DMeshVolume@@@0@00V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z); STLport's pair is a struct (PAU), as the sort front end
//   now binds it. Now the STLport 4.6 body of stlport/fable_containers.h by explicit instantiation over engine/C3DMeshVolume.h and the global
//   comparator of engine/CVectorMap.h (the 0x00A33870 recipe); `// Flags: /Gr`. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_UNGUARDED_INSERTION_SORT_AUX_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void __fastcall std::__unguarded_insertion_sort_aux(std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,CKeyPairCompareLess<long,C3DMeshVolume>);
