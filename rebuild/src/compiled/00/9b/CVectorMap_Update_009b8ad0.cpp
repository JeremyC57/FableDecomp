// CVectorMap<unsigned long, unsigned long, CKeyPairCompareLess<unsigned long, unsigned long> >::Update() -- when Dirty, sort the
//   pairs with the stored comparator and clear Dirty
// Leaf: ?Update@?$CVectorMap@KKV?$CKeyPairCompareLess@KK@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B8AD0: `cmp byte ptr [esi+0Dh],0 ; je` around the inlined Sort (the /Gr std::sort 0x009B85A0
//   with the comparator byte pushed, then `mov byte ptr [esi+0Dh],0`).
// Identity (NF53): FableWin.pdb ?Update@?$CVectorMap@KKV?$CKeyPairCompareLess@KK@@@@QAEXXZ; the body is engine/CVectorMap.h's Update
//   (`if (Dirty) Sort();`, FABLE_RETAIL_CVECTORMAP_SORT), the sibling of Sort 0x009B89B0. Was the stand-in CTCVillage::OnInitialActivate
//   / InnerActivate_009b85a0 (same bytes).
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_SORT
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
template void CVectorMap<unsigned long, unsigned long>::Update();
