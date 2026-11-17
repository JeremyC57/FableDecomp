// CVectorMap<unsigned long, unsigned long, CKeyPairCompareLess<unsigned long, unsigned long> >::Sort() -- sort the pairs with the
//   stored comparator (the /Gr std::sort 0x009B85A0: first in ecx, last in edx, the comparator byte pushed), then clear Dirty
// Leaf: ?Sort@?$CVectorMap@KKV?$CKeyPairCompareLess@KK@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B89B0, `mov edx,[esi+4] ; mov ecx,[esi] ; xor eax,eax ; mov al,[esi+0Ch] ; push eax ;
//   call 0x009B85A0 ; mov byte ptr [esi+0Dh],0`.
// Identity (NF53): Ego_r.pdb / FableWin.pdb ?Sort@?$CVectorMap@KKV?$CKeyPairCompareLess@KK@@@@QAEXXZ; the body is engine/CVectorMap.h's
//   Sort (std::sort over [begin, end) with Compare +0x0C, then Dirty +0x0D cleared, no Dirty test). The sort it calls is
//   ??$sort@PAU?$pair@KK@std@@V?$CKeyPairCompareLess@KK@@@std@@YIX..@Z (the queue claim, nf44:97). Was the stand-in
//   CGuiVarTransferBool::TransferToGui / CGuiVarTransferBool_Sink_009b85a0 (same bytes).
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_SORT
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
template void CVectorMap<unsigned long, unsigned long>::Sort();
