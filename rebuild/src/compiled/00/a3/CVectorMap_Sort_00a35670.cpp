// CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess<unsigned long, C3DMeshVolume> >::Sort() -- sort the pairs by key with the stored comparator, then clear Dirty
// Leaf: ?Sort@?$CVectorMap@KVC3DMeshVolume@@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35670, 25 bytes.
// Identity: FableWin.pdb ?Sort@?$CVectorMap@KVC3DMeshVolume@@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@@@QAEXXZ (the Lionhead CVectorMap template member; Ego_r.pdb has the same
//   instance's LowerBound / UpperBound), out of line here for the C3DMeshStats volume table: it calls std::sort 0x00A355F0 with the map's begin / end and Compare (+0x0C) and clears Dirty (+0x0D). The callee chain proves the element
//   (std::sort 0x00A355F0 -> __introsort_loop 0x00A35490 over pair<unsigned long, C3DMeshVolume>).
// Shape: `push Compare ; call std::sort ; mov byte ptr [esi+0Dh],0`.
// NF91 (2026-11-17, q11190): the old row's stand-in models (CVectorMap_T_16 / free OnInitialActivate(CTCVillage*)) are gone; re-authored on
//   engine/CVectorMap.h (FABLE_RETAIL_CVECTORMAP_SORT: the inline Sort / Update bodies, the 0x00A42040 / 0x00A420C0 recipe) with std::sort
//   declared __fastcall (FABLE_RETAIL_STLP_SORT_FASTCALL); explicit instantiation of the member.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_SORT
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess<unsigned long,C3DMeshVolume> >::Sort();
