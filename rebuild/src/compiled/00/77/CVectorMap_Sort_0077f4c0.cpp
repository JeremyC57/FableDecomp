// CVectorMap<CCharString, C3DVector, CKeyPairCompareLess<CCharString, C3DVector> >::Sort() -- sort the pairs by key with the stored comparator, then clear Dirty
// Leaf: ?Sort@?$CVectorMap@VCCharString@@VC3DVector@@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077f4c0, 25 bytes.
// Identity: FableWin.pdb ?Sort@?$CVectorMap@VCCharString@@VC3DVector@@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@@QAEXXZ (this
//   instance), out of line here: it calls std::sort 0x0077F440 (pair<CCharString, C3DVector>; NF87) with the map's begin / end and Compare
//   (+0x0C) and clears Dirty (+0x0D). The old row's CGuiVarTransferBool_TransferToGui / GuiVarBoolHelper models were stand-ins.
// Shape: `push Compare ; call std::sort ; mov byte ptr [esi+0Dh],0`.
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
template void CVectorMap<CCharString, C3DVector, CKeyPairCompareLess<CCharString, C3DVector> >::Sort();
