// std::__insertion_sort(std::pair<CCharString,C3DMeshVolume>* first, std::pair<CCharString,C3DMeshVolume>* last, CKeyPairCompareLess<CCharString,C3DMeshVolume> comp) -- straight insertion sort: hand each entry after the first by value to __linear_insert (the name-keyed C3DMeshVolume map (CVectorMap<CCharString, C3DMeshVolume>))
// Leaf: ??$__insertion_sort@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A732B0, 113 bytes (__fastcall: first / last in ecx / edx, the rest pushed).
// Identity: the name-keyed C3DMeshVolume map (CVectorMap<CCharString, C3DMeshVolume>) sort family (std::sort 0x00A73BA0, NF91 q11191 / q11192). Callees include 0x00A729B0.
// NF91 (2026-11-17, claimed as the q11191 / q11192 family follow-up): the old row spelt the element as a row-local `class pair` (PAV) on local
//   CCharString / C3DMeshVolume / CKeyPairCompareLess models (own leaf ??$__insertion_sort@PAV?$pair@VCCharString@@VC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIXPAV?$pair@VCCharString@@VC3DMeshVolume@@@0@0V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z); STLport's pair is a struct (PAU), as the sort front end
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
template void __fastcall std::__insertion_sort(std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,CKeyPairCompareLess<CCharString,C3DMeshVolume>);
