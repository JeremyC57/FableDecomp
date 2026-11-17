// std::__unguarded_linear_insert(std::pair<CCharString,C3DMeshVolume>* last, std::pair<CCharString,C3DMeshVolume> val, CKeyPairCompareLess<CCharString,C3DMeshVolume> comp) -- insertion-sort inner step without a lower-bound check: shift every larger entry up one slot, drop the value into the gap, destroy the by-value copy (the name-keyed C3DMeshVolume map (CVectorMap<CCharString, C3DMeshVolume>))
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DMeshVolume@@@0@U10@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70540, 239 bytes (__fastcall: first / last in ecx / edx, the rest pushed).
// Identity: the name-keyed C3DMeshVolume map (CVectorMap<CCharString, C3DMeshVolume>) sort family (std::sort 0x00A73BA0, NF91 q11191 / q11192).
// NF91 (2026-11-17, claimed as the q11191 / q11192 family follow-up): the old row spelt the element as a row-local `class pair` (PAV) on local
//   CCharString / C3DMeshVolume / CKeyPairCompareLess models (own leaf ??$__unguarded_linear_insert@PAV?$pair@VCCharString@@VC3DMeshVolume@@@std@@V12@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIXPAV?$pair@VCCharString@@VC3DMeshVolume@@@0@V10@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z); STLport's pair is a struct (PAU), as the sort front end
//   now binds it. Now the STLport 4.6 body of stlport/fable_containers.h by explicit instantiation over engine/C3DMeshVolume.h and the global
//   comparator of engine/CVectorMap.h (the 0x00A33250 recipe); `// Flags: /Gr`. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#define FABLE_RETAIL_STLP_UNGUARDED_LINEAR_INSERT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__unguarded_linear_insert(std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>,CKeyPairCompareLess<CCharString,C3DMeshVolume>);
