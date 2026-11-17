// std::__linear_insert(std::pair<long,C3DMeshVolume>* first, std::pair<long,C3DMeshVolume>* last, std::pair<long,C3DMeshVolume> val, CKeyPairCompareLess<long,C3DMeshVolume> comp) -- insertion-sort step: a value smaller than the first entry shifts the run up one slot and goes first, anything else goes through the unguarded insert; the by-value val is destroyed (the CPhysicsMesh navigation containment volumes (CVectorMap<long, C3DMeshVolume>))
// Leaf: ??$__linear_insert@PAU?$pair@JVC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIXPAU?$pair@JVC3DMeshVolume@@@0@0U10@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A728F0, 179 bytes (__fastcall: first / last in ecx / edx, the rest pushed).
// Identity: the CPhysicsMesh navigation containment volumes (CVectorMap<long, C3DMeshVolume>) sort family (std::sort 0x00A73B20, NF91 q11191 / q11192). Callees include 0x00A70A10 / 00A703C0.
// NF91 (2026-11-17, claimed as the q11191 / q11192 family follow-up): the old row spelt the element as a row-local `class pair` (PAV) on local
//   CCharString / C3DMeshVolume / CKeyPairCompareLess models (own leaf ??$__linear_insert@PAV?$pair@JVC3DMeshVolume@@@std@@V12@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@std@@YIXPAV?$pair@JVC3DMeshVolume@@@0@0V10@V?$CKeyPairCompareLess@JVC3DMeshVolume@@@@@Z); STLport's pair is a struct (PAU), as the sort front end
//   now binds it. Now the STLport 4.6 body of stlport/fable_containers.h by explicit instantiation over engine/C3DMeshVolume.h and the global
//   comparator of engine/CVectorMap.h (the 0x00A34820 recipe); `// Flags: /Gr`. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__linear_insert(std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>*,std::pair<long,C3DMeshVolume>,CKeyPairCompareLess<long,C3DMeshVolume>);
