// CVectorMap<CCharString, CSoundBankLH*, CKeyPairCompareLess<CCharString, CSoundBankLH*> >::Update() -- re-sort the map when it is Dirty (Sort expanded in place), then clear Dirty
// Leaf: ?Update@?$CVectorMap@VCCharString@@PAVCSoundBankLH@@V?$CKeyPairCompareLess@VCCharString@@PAVCSoundBankLH@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00a420c0, 32 bytes.
// Identity: the Ego_r.pdb / FableWin.pdb CVectorMap::Update member (FableWin.pdb: 54 ?Update@?$CVectorMap@..@@QAEXXZ and the ?Sort@ family, e.g.
//   ..@VCCharString@@VC3DVector@@..), out of line here for the sound manager's map<CCharString, CSoundBankLH*>: it calls std::sort 0x00A41E20
//   (the same pair instance; NF87) with the map's begin / end and Compare (+0x0C) and clears Dirty (+0x0D). No PDB holds this instance's
//   own symbol (descriptive-by-library: the member template over the element the sort callee proves). The old row's
//   free OnInitialActivate(CTCVillage*) / Village_Sub models were stand-ins.
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
class CSoundBankLH;   // RTTI .?AVCSoundBankLH@@ (no header: declared only)
#include "engine/CVectorMap.h"
template void CVectorMap<CCharString, CSoundBankLH*, CKeyPairCompareLess<CCharString, CSoundBankLH*> >::Update();
