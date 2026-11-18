// CVectorMap<CCharString, C3DVector, CKeyPairCompareLess<CCharString, C3DVector> >::Update() -- re-sort the map when it is Dirty (Sort expanded in place), then clear Dirty
// Leaf: ?Update@?$CVectorMap@VCCharString@@VC3DVector@@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077f4e0, 32 bytes.
// Identity: FableWin.pdb ?Update@?$CVectorMap@VCCharString@@VC3DVector@@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@@QAEXXZ (this
//   instance), out of line here: it calls std::sort 0x0077F440 (pair<CCharString, C3DVector>; NF87) with the map's begin / end and Compare
//   (+0x0C) and clears Dirty (+0x0D). The old row's OnInitialActivate(CTCVillage*) / InnerActivate_0077f440 models were stand-ins.
// Shape: `cmp byte ptr [Dirty],0 ; je`, then the inline Sort.
// NF87: re-authored on engine/CVectorMap.h (FABLE_RETAIL_CVECTORMAP_SORT: the inline Sort / Update bodies) with std::sort declared
//   __fastcall (FABLE_RETAIL_STLP_SORT_FASTCALL); explicit instantiation of the member.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_SORT
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
template void CVectorMap<CCharString, C3DVector, CKeyPairCompareLess<CCharString, C3DVector> >::Update();
